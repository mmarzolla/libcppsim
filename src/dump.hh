/*****************************************************************************
 *
 * mean.hh
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

#ifndef DUMP_HH
#define DUMP_HH

#include <fstream>
#include "statistic.hh"

class dump : public statistic {
public:
    dump( const string &name );
    virtual ~dump( );
    void update( double v );
    void reset( void );
    void report( void ) const { };
protected:
    confInt value( void );

    ofstream os;
};

#endif
