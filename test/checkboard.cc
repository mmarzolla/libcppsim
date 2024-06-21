/****************************************************************************
 *
 * checkboard.cc
 *
 * This file is part of cppsim
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

/*
 * This program draws an 8 by 8 checker board on the screen.
 * The checker board is made of alternating 'X' and 'O' symbols.
 */
#include "coroutine.hh"
#include <iostream>

using namespace std;

int _pos = 0;

class position : public coroutine {
public:
    position* next;
    position( char s ) :
        coroutine( ),
        _s( s )
    { };
    virtual ~position( ) { };
protected:
    void main( void ) {
        while( _pos++ < 64 ) {
            cout << _s;
            if ( 0 == _pos % 8 )
                cout << endl;
            else 
                next->resume( );
        }        
        cout << "finished" << endl;
    }
    const char _s;
};

int main( void )
{
    position* p1 = new position( 'O' );
    position* p2 = new position( '.' );
    p1->next = p2;
    p2->next = p1;
    p1->resume( );
}
