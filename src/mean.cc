/*****************************************************************************
 *
 * mean.cc -- Compute the mean of a sequence of UNCORRELATED observations
 * x_1, x_2, \ldots x_N
 *
 * Copyright (C) 2002, 2003, 2024 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *****************************************************************************/

#include <cmath>
#include <cstdio>
#include <vector>
#include "mean.hh"
#include "statfun.hh"
#include "assertions.hh"

mean::mean( const string& name, double confl ) :
    statistic   ( name, confl ),
    _sum        ( 0.0 ),
    _sumsq      ( 0.0 )
{

}

mean::~mean( )
{

}

void mean::update( double v )
{
    _sum += v;
    _sumsq += squared(v);
    _numUpdates++;
}

void mean::reset( void )
{
    _sum = _sumsq = 0.0;
    _numUpdates = 0;
    _numResets++;
}

confInt mean::value( void )
{
    //
    // Banks, p. 227
    //
    t_assert( numUpdates() > 0,
              runtime_error( "Computing the mean with no observations" ) );
    double w = qnorm( 1.0 - alpha()/2.0 )*sqrt( variance()/(double)numUpdates() );
    return confInt( m()-w, m()+w, confl() );
}

double mean::m( void ) const
{
    t_assert( numUpdates() > 0,
              runtime_error( "Computing the mean with no observations" ) );
    return _sum / (double)numUpdates();
}

double mean::variance( void ) const
{
    t_assert( numUpdates() > 1,
              runtime_error( "Computing the variance with less than 2 observations" ) );
    //
    // Note: fabs() is necessary as numerical errors may cause the
    // variance to be a tiny negative number. In this way the sqrt()
    // function on function mean() will fail and cause problems
    //
    return fabs(
                ( _sumsq - _sum*_sum / (double)numUpdates( ) ) /
                (double)( numUpdates( )-1 )
                );
}

void mean::report( void ) const
{
    confInt c = const_cast<mean*>(this)->value( );
    printf( "%15s / %6s / %8s / %8s\n"
            "%15s / %6d / %8.4f / %8.4f\n",
            "Name", "Obs", "lBound", "uBound",
            name( ).c_str( ), numUpdates( ), c.lBound( ), c.uBound( ) );
}
