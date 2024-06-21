/*****************************************************************************
 *
 * abatch.cc
 *
 * Implementation of the ABATCH strategy
 *
 * Copyright (C) 2002,2003 Moreno Marzolla
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****************************************************************************/

#include <cmath>
#include <iostream>
#include <cstdio>
#include <vector>
#include "abatch.hh"
#include "statfun.hh"

//
// Computes the approximate base-2 logarithm of n. Approximate means that
// the value computed is the greatest integer x such that 2^x <= n.
//
unsigned int log2( unsigned int n )
{
    unsigned int result = 0;
    while ( (unsigned int)( 1 << result ) <= n )
        result++;
    return result;
}

abatch::abatch( const string& name, double confp, 
		unsigned int sampsize, unsigned int transientLen ) :
    statistic( name, confp ),
    alpha( 1.0 - confp ),
    b_1( 1 ),          // See Banks, p. 255
    k_1( 16 ),         // See Banks, p. 255
    n( sampsize ),
    transientLen( transientLen ),
    sum( 0.0 ),
    os( name.c_str( ) )
{
    b = b_1;
    k = k_1;
    btilde_1 = ( b_1 == 1 ? 3.0/2.0 : floor( sqrt( 2.0 )*b_1+0.5 ) );
    ktilde_1 = (unsigned int)floor( sqrt(2.0)*k_1 + 0.5 );
    L = log2( sampsize / (k_1*b_1) );
    g =  btilde_1 / b_1;
    f =  ktilde_1 / k_1;
    ntilde = (unsigned int)( ( 1 << ( L-1 ) )*ktilde_1*btilde_1 );
    N = k*b;
#ifdef PIPPO
    os << form( "%8s %8s %8s %8s %8s %8s %8s",
                "N", "k", "b", "xbar", "xbar-d", "xbar+d", "vb" ) 
       << endl;
#endif
}

abatch::~abatch( )
{

}

void abatch::update( const double& v )
{
    ob.push_back( v );
    // _numUpdates++;
    statistic::update( v );
    sum += v;
    if  ( numUpdates() == N ) {
        process_batch( );
        N = k*b;
        cout << "Next review at observation " << N << endl;
    }
}

void abatch::process_batch( void )
{
    vector<double> Y( k, 0.0 );   // Vector of batch means
    unsigned int ii, jj;
    double xbar = 0.0;
    double vb = 0.0;
    double delta;
    double p;
    bool H0_accepted;

    cout << ob.size( ) << " " << b << " " << k << endl;

    assert( ob.size( ) == numUpdates() );
    assert( ob.size( ) == b * k );

    xbar = sum / (double)numUpdates();

    // Compute Y[ii] as the means of the batch
    for ( ii=0; ii<k; ii++ ) 
        for ( jj=0; jj<b; jj++ )
            Y[ii] += ob[ ii*b + jj ];

    // Compute vb
    for ( ii=0; ii<k; ii++ )
	vb += squared( Y[ii] - xbar )/(double)(k-1);
     
    delta = qt( 1.0 - alpha/2.0, k-1, true ) * sqrt( vb / (double)k );

    H0_accepted = H0_is_accepted( Y, xbar, p );

#ifdef PIPPO
    os << form( "%8d %8d %8d %8.4f %8.4f %8.4f %8e %8e",
                N, k, b, xbar, xbar-delta, xbar+delta, vb, p )
       << flush << endl;
#endif

    if ( H0_accepted ) {                        // H0 is accepted
        if ( b == 1 )                          // (FNB rule)
            b = 2;                             
        else {                                  // (SQRT rule)
            b = ( unsigned int )( b*g );
            k = ( unsigned int )( k*f );
            if ( g == btilde_1/b_1 ) {
                g = 2.0*b_1 / btilde_1;
                f = 2.0*k_1 / ktilde_1;
            } else {
                g = (double)btilde_1 / b_1;
                f = (double)ktilde_1 / k_1;
            }
        }
    } else {
        b = 2*b;                              // (FNB rule)
    }
}

bool abatch::H0_is_accepted( const vector<double> &Y, const double xbar,
                             double& p )
{
    unsigned int kk = Y.size( );
    unsigned int ii;
    double ro_num = 0.0, ro_den = 0.0;
    double ro;
    double gamma_k;
    const double beta = 0.05;

    for ( ii=0; ii<kk-1; ii++ )
        ro_num += (Y[ii]-xbar)*(Y[ii+1]-xbar);
    for ( ii=0; ii<kk; ii++ )
        ro_den += squared(Y[ii]-xbar);
    ro = ro_num / ro_den;

    gamma_k = sqrt( ( squared(kk)-1.0 )/( kk-2.0 ) ) *
        ( ro + ( squared( Y[0]-xbar ) + squared( Y[kk-1]-xbar ) ) / (2.0 * ro_den ) );

    if ( gamma_k < qnorm( 1.0-beta ) )
        cout << "Test accepted" << endl;
    else
        cout << "Test rejected" << endl;
    p = 1.0 - qnorm( gamma_k );
    return ( gamma_k < qnorm( 1.0 - beta ) );
}

void abatch::reset( void )
{
    ob.clear( );
    // _numUpdates = 0;
    // _numResets++;
    statistic::reset( );
}

void abatch::report( void ) const 
{

}

confInt abatch::value( void )
{
    return confInt( 0,0,0 );
}
