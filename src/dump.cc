/*****************************************************************************
 *
 * dump.hh
 *
 * Dumps a sequence of observations X_0, X_1, \ldots X_N into a file
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

#if HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <iostream>
#include <string>
#include <cstdlib>
#include "dump.hh"

dump::dump( const string& name ) :
    statistic( name, 0.99 ),
    os( string(name+".dump").c_str() )
{
    os << "# NumResets numUpdates value" << endl;
}

dump::~dump( )
{

}

void dump::update( double v )
{
    os << numResets( ) << " " << numUpdates( ) << " " << v << endl << flush;
    _numUpdates++;
}

void dump::reset( void )
{
    _numUpdates = 0;
    _numResets++;
}

confInt dump::value( void )
{
    cerr << "FATAL: Trying to call dump::value()" << endl;
    abort( );
    return confInt( 0.0, 0.0, 0.0 );
}
