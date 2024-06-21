/****************************************************************************
 *
 * bmeans2.cc
 *
 * This file is part of libcppsim
 *
 * Copyright (C) 2003, 2004 Moreno Marzolla
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 ****************************************************************************/

#include <cmath>
#include <cstdio>
#include "bmeans2.hh"
#include "assertions.hh"
#include "statfun.hh"

//
// Implementation of the batch means method.
// This implementation is described on p. 437 of:
//
// J. Banks, J. S. Carson, B. L. Nelson, D. M. Nicol,
// "Discrete-Event System Simulation",
// 3rd ed, Prentice Hall, 2000      ISBN 0130887021 
//
// I tried to use more or less the same name for the
// variables. Remember, however, that indices of the vectors, in this
// implementation, start from 0 (not 1 as in the reference above). So
// the vector of the batch means Y_bar goes from 0 to k-1 (instead of
// 1 to k).
//

// z_{\alpha/2} is the 100(1-\alpha/2) percentile of the normal distribution.
#define Z( alpha_2 ) qnorm( 1.0 - (double)(alpha_2) )

// t_{\alpha/2, f} is the 100( 1 - \alpha / 2 ) percentile of 
// a Student's t distribution.
#define T( alpha_2, f ) qt( 1.0 - (double)(alpha_2), f, true )

bmeans2::bmeans2( const string& name, double confp, trremoval* t ) :
    statistic( name, confp ),
    _t( t ? 
        t : 
        new trremoval_frac( "bmeans(" + name + ")::_t", .2 ) )
{
    assert( 0 != _t );
}

bmeans2::~bmeans2( )
{
    delete _t;
}

void bmeans2::update( double v )
{
    _t->update( v );
    _numUpdates++;    
}

void bmeans2::reset( void )
{
    _t->reset();
    _numUpdates = 0;
    _numResets++;
}

void bmeans2::report( void ) const throw( runtime_error )
{
    //    confInt v = const_cast<bmeans2*>( this )->value();
    //    cout << v.lBound() << "-" << v.uBound() << endl;
}

confInt bmeans2::value( void ) throw( runtime_error )
{
    unsigned int k = 200;       // Number of batches
    unsigned int b;             // btach size
    int i, j;
    char msg[ 1024 ];

    vector<double> _ob( _t->value() ); // Get the observations
    snprintf( msg, sizeof( msg ), "Get more observations (currently got %u)",
              numUpdates() );
    t_assert( _ob.size() > 4*k, runtime_error( msg ) );
    b = _ob.size() / k;
    _ob.resize( k*b );          // Cut off last observations

    // Vector of sums
    vector<double> _sum( k*b+1, 0.0 );
    for ( i=1; i<k*b+1; i++ )
        _sum[i] = _sum[i-1]+_ob[i-1];

    // vector of means
    vector<double> Y_bar( k, 0.0 );
    for ( i=0; i<k; i++ )        
        Y_bar[i] = ( _sum[ (i+1)*b ] - _sum[ i*b ] ) / (double)b;
    
    // Overall mean
    double _mean = _sum[ k*b ] / (double)(k*b);
    
    // Compute the lag-1 autocorrelation
    double ro_1_num = 0.0;
    double ro_1_den = 0.0;
    for ( j=0; j<k-1; j++ )
        ro_1_num += ( Y_bar[j] - _mean ) * ( Y_bar[j+1] - _mean );
    for ( j=0; j<k; j++ )
        ro_1_den += ( Y_bar[j] - _mean ) * ( Y_bar[j] - _mean );
    double ro_1 = ro_1_num / ro_1_den;
    
    snprintf( msg, sizeof( msg ), 
              "lag-1 autocorrelation too big (is %e, should be at most 0.2)", ro_1 );
    t_assert( ro_1 < 0.2, runtime_error( msg ) );

    // Rebatch into 40 batches, and compute the result
    k = 40;
    b = _ob.size() / k;
    Y_bar.resize( k );
    for ( i=0; i<k; i++ )
        Y_bar[i] = ( _sum[ (i+1)*b-1 ] - _sum[ i*b ] ) / (double)b;

    // Check the batch means for independence
    double C =
        sqrt( (double)( squared(k)-1)/(double)(k-2) ) *
        ( ro_1 + 
          (squared( Y_bar[0] - _mean )+squared( Y_bar[k-1] - _mean)) /
          ( 2.0 * ro_1_den ) );

    snprintf( msg, sizeof( msg ), 
              "Independence test failed (C=%e, at most qnorm=%e)", C, Z( 0.05 ) );
    t_assert( C < Z( 0.05 ), runtime_error( msg ) );
    
    // Compute S^2/k
    double S_2_k = 0.0;
    for ( j=0; j<k; j++ )
        S_2_k += squared( Y_bar[i]-_mean );
    S_2_k /= (double)k*(k-1);

    // Output the result!
    return confInt( _mean - T( alpha() / 2.0, k-1 ) * sqrt( S_2_k ),
                    _mean + T( alpha() / 2.0, k-1 ) * sqrt( S_2_k ), confl() );

}
