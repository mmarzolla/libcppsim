/*****************************************************************************
 *
 * accum.cc
 *
 * Definition of an accumulator class.
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

#include "accum.hh"
#include <cstdio>

accum::accum( const string& name ) :
    var_impl( name ),
    _sum( 0.0 ),
    _lastUpdate( 0.0 ),
    _lastVal( 0.0 )
{

}

accum::~accum( )
{

}

void accum::update( const pair< double, double >& val_time )
{
    const double v = val_time.first;
    const double t = val_time.second;
    assert( t >= _lastUpdate );
    _sum += _lastVal*( t - _lastUpdate );
    _lastUpdate = t;
    _lastVal = v;
    _numUpdates++;
}

void accum::reset( void )
{
    _sum = 0.0;
    _lastUpdate = 0.0;
    _lastVal = 0.0;
    _numUpdates = 0;
    _numResets++;
}

void accum::report( void ) const
{
    printf( "%15s / %6s / %8s\n"
            "%15s / %6d / %8.2f\n\n",
            "Name", "Obs", "Sum",
            name().c_str( ), numUpdates(), _sum );
}

