/****************************************************************************
 *
 * simulation.cc
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

#include "simulation.hh"
#include "sqs.hh"

simulation* simulation::_instance = 0;

void simulation::begin_simulation( sqs* s ) 
{
    assert( 0 != s );
    _sqs.push_front( s );
}

void simulation::end_simulation( void )
{
    assert( ! _sqs.empty() );
    sqs* s = _sqs.front( );
    _sqs.pop_front( );
    delete s;
}

sqs* simulation::getSqs( void )
{
    assert( ! _sqs.empty() );
    return _sqs.front( );
}

void simulation::run( void )
{
    assert( ! _sqs.empty() );
    _sqs.front( )->call( );
}

simulation* simulation::instance( void )
{
    if ( 0 == _instance )
        _instance = new simulation( );
    return _instance;
}

unsigned long simulation::events( void ) const
{
    assert( ! _sqs.empty() );
    return _sqs.front()->processed( );
}
