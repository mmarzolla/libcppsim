/****************************************************************************
 *
 * logger.cc
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

#include "logger.hh"
#include <iostream>
#include <cstdio>
#include <cassert>

// using namespace std;

map< string, logger* > logger::_instances = map< string, logger* >();

logger::logger( ) :
    _os ( &cerr ),
    _l  ( -1 )
{

}

logger* logger::instance( const string& name )
{
    if ( 0 == _instances[ name ] ) 
        _instances[ name ] = new logger( );
    return _instances[ name ];
}

void logger::setOutputFile( std::ostream* os )
{
    assert( 0 != os );
    _os = os;
}

void logger::message( int l, const char* fmt, ... )
{
    static char msgBuf[ 128 ];

    va_list ap;
    va_start( ap, fmt );
    if ( l <= _l ) {
        vsnprintf( msgBuf, sizeof( msgBuf ), fmt, ap );
        *_os << msgBuf;
    }
    va_end( ap );    
}

void logger::vmessage( int l, const char* fmt, va_list ap )
{
    static char msgBuf[ 128 ];

    if ( l <= _l ) {
        vsnprintf( msgBuf, sizeof( msgBuf ), fmt, ap );
        *_os << msgBuf;
    }
}
