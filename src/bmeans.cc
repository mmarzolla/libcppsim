/*****************************************************************************
 *
 * bmeans.cc
 *
 * Batch Means Method
 *
 * This file is part of libcppsim
 *
 * Copyright (C) 2002, 2003 Moreno Marzolla
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
#include <vector>
#include <cstdio>
#include "bmeans.hh"
#include "statfun.hh"
#include "assertions.hh"

bmeans::bmeans( const string& name, double confl, unsigned int bsize, trremoval* t ) :
    statistic   ( name, confl ),
    _bsize      ( bsize ),
    _t          ( t ? 
                  t : 
                  new trremoval_const( "bmeans(" + name + ")::_t", 0 ) )
{
    assert( _bsize > 0 );
    assert( 0 != _t );
}

bmeans::~bmeans( )
{
    delete _t;
}

void bmeans::update( double v )
{
    _numUpdates++;
    _t->update( v );
}

void bmeans::reset( void )
{
    _t->reset( );
    _numUpdates = 0;
    _numResets++;
}

//
// Computes the mean and variance of the sequence of observations, using
// batches of size bsize. The autocovariance of the batches is also shown.
// 
confInt bmeans::precompute( int bsize,          // Batch size, >= 1
			    double p,           // 1-\alpha conf probability
			    double& variance,   // variance
			    double& cov         // covariance
			    ) const throw ( runtime_error )
{
    vector<double> _ob( _t->value( ) );
    int i,j;
    int k      = _ob.size( ) / bsize; // Number of batches
    t_assert( k > 2,
              runtime_error( "Batch means with less than 2 batches" ) );
    vector< double > Y( k, 0.0 );
    double mean = 0.0;                // Mean
    double width = 0.0;               // Confidence interval half-sidth

    variance = cov = 0.0;

    for ( i=0; i<k; i++ ) {
	Y[i] = 0.0;
	for ( j = i*bsize; j < (i+1)*bsize; j++ )
	    Y[i] += _ob[j] / bsize;
	mean += Y[i]/k;
    }

    for ( i=0; i<k; i++ )
	variance += ( Y[i] - mean )*( Y[i] - mean )/(double)(k-1);

    for ( i=0; i<k-1; i++ )
        cov += squared(Y[i]-mean)/(double)(k-2);

    width = qt( 1.0 - alpha()/2.0, k-1, true ) * sqrt( variance / (double)k );

    return confInt( mean-width, mean+width, p );
}

void bmeans::report( void ) const throw( runtime_error )
{
    unsigned int bsize; // Batch size
    vector<double> _ob = _t->value( );
    cout << "Batch means for: " << name() << endl;
    printf( "%4s / %6s / %10s / %10s / %10s / %23s\n",
            "b", "N", "mean", "variance", "autocov", "conf. interval" );
           
    for ( bsize = 16; bsize < _ob.size( ) / 30; bsize = bsize << 1 ) {

        double variance, cov;
        confInt c = precompute( bsize, confl(), variance, cov );

        printf( "%4d / %6d / %10.4f / %10.4f / %10.4f / %10.4f - %10.4f\n",
                bsize, _ob.size(), c.val( ), variance, cov, c.lBound( ), c.uBound( ) );
    }
}

confInt bmeans::value( void ) throw( runtime_error )
{
    double variance, cov;
    return precompute( _bsize, confl(), variance, cov );
}
