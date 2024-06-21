/****************************************************************************
 *
 * sqs.cc
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

#include "sqs.hh"
#include <cstdio>
#include "sqsDll.hh"
#include "sqsPrio.hh"
#include "logger.hh"

unsigned int sqs::_id = 0;

sqs::sqs( ) :
    coroutine   ( ),
    _insertions ( 0 ),
    _deletions  ( 0 ),
    _processed  ( 0 ),
    _current    ( )
{
    // FIXME: Is there an easy way for concatenating an integer to a string,
    // without using snprintf??
    char buf[128];
    snprintf( buf, sizeof( buf ), "libcppsim::sqs%u", _id++ );
    _logger = logger::instance( buf );
}

handle_p& sqs::firstProc( void ) const 
{
    static handle_p nullProc( 0 );
    absEvNotice* _ev = first( );
    return ( 0 == _ev ? nullProc : _ev->proc( ) );
}

void sqs::main( void ) 
{
    while( !empty( ) ) {
        absEvNotice* _ev = first( );
        assert( 0 != _ev );
        _current = _ev->proc( );
        _current->resume( );
        _processed++;
        _logger->message( 0, "%u\n", size() );
    }
}

sqs* makeSqs( sched_t s )
{
    switch ( s ) {
    case SQSDLL:
        return new sqsDll();
        break;
    case SQSPRIO:
        return new sqsPrio();
        break;
    default:
        cerr << "Unsupported SQS type " << s << endl;
        abort( );
    };
}

sqsBuilder_t builders[2] = {
    sqsDllBuilder,
    sqsPrioBuilder
};

