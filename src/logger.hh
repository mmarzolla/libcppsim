/****************************************************************************
 *
 * logger.hh
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

#ifndef LOGGER_HH
#define LOGGER_HH

#include <cstdarg>
#include <iostream>
#include <map>
#include <string>
class logger;

using namespace std;

class logger {
public:
    virtual ~logger( ) { };
    static logger* instance( const string& name );

    void setLevel( int l ) { _l = l; };
    bool enabled( int l ) const { return ( l <= _l ); };
    void setOutputFile( ostream* os );
    void message( int l, const char* fmt, ... );
    void vmessage( int l, const char* fmt, va_list ap );
protected:
    logger( );

    ostream*    _os;    // Output stream
    int         _l;     // Level of verbosity
    static map< string, logger* > _instances;
};

#endif
