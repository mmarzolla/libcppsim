/****************************************************************************
 *
 * handleTest.cc
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <iostream>
#include "handle.hh"

using namespace std;

class foo : public shared {
public:
    foo( const string& name ) :
        shared( ),
        _name( name )
    { 
        cout << "Created foo'" << _name << "'" << endl;
    };

    virtual ~foo( )
    {
        cout << "Destroyed foo '" << _name << "'" << endl;
    };
protected:
    const string _name;
};

int main( void )
{
    handle<foo> one( new foo("one") );
    handle<foo> two( new foo("two") );
    for ( int i=0; i<5; i++ ) {
        handle<foo> three( new foo("three"+string(i,'#')) );
        one = three;
    }
    return 0;
}
