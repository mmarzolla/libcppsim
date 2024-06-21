/****************************************************************************
 *
 * quantile.cc
 *
 * This file is part of libcppsim
 *
 * Copyright (C) 2003 Moreno Marzolla
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

#include "quantile.hh"
#include <cassert>
#include <algorithm>
#include <cmath>
#include "statfun.hh"

quantile::quantile( const string& name, double p, double confl ) :
    statistic( name, confl ),
    _p( p )
{
    assert( p > 0.0 );
    assert( p < 1.0 );
}

quantile::~quantile( )
{

}

void quantile::update( double v )
{
    _numUpdates++;
    _ob.push_back( v );
}

void quantile::reset( void )
{
    _numResets++;
    _numUpdates = 0;
    _ob.clear( );
}

confInt quantile::value( void )
{
    // Sort the observations
    sort( _ob.begin(), _ob.end() );
    
    unsigned int k = _ob.size();

#ifdef SKIP
    //
    // Step 1: compute the central value
    //
    double result;
    if ( floor(k*_p) == k*_p )          // if k*p is integer
        result = _ob[ (unsigned int) (k*_p) ];
    else                                // if k*p is not integer
        result = _ob[ (unsigned int)ceil( k*_p+1.0 ) ];
#endif

    //
    // Step 2: compute the confidence interval
    //
    unsigned int i,j;
    i = (unsigned int)ceil( k*_p + 1.0 -
                            qnorm( 1.0 - alpha()/2.0 )*sqrt( k*_p*(1.0-_p) ) );
    j = (unsigned int)ceil( k*_p + 1.0 +
                            qnorm( 1.0 - alpha()/2.0 )*sqrt( k*_p*(1.0-_p) ) );
    return confInt( _ob[i], _ob[j], confl() );
}

void quantile::report( void ) const
{

}
