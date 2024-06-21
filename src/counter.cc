/*****************************************************************************
 *
 * counter.cc
 *
 * Definition of a counter class.
 *
 * Copyright (C) 2002 Moreno Marzolla, Paolo Palmerini
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

#include <iostream>
#include <cstdio>
#include "counter.hh"

counter::counter( const string& name ) :
    var_impl    ( name ),
    _min        ( -1 ),
    _max        ( -1 ),
    _sum        ( 0.0 )
{

}

counter::~counter( )
{

}

void counter::update( double v )
{
    if ( -1 == _min || v < _min )
	_min = v;
    if ( -1 == _max || v > _max )
	_max = v;
    _sum += v;
    _numUpdates++;
}

void counter::reset( void )
{
    _min = _max = -1;
    _sum = 0.0;
    _numUpdates = 0;
    _numResets++;
}

void counter::report( void ) const
{
    printf( "%15s / %6s / %8s / %8s / %8s\n"
            "%15s / %6d / %8.2f / %8.2f / %8.2f\n\n",
            "Name", "Obs", "Sum", "min", "max",
            name().c_str( ), numUpdates(), _sum, _min, _max );
}
