/****************************************************************************
 *
 * pcheckboard.cc
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
 */
#include <iostream>
#include "cppsim.hh"

class draw_site : public process {
public:
    draw_site( char s, double d ) :
        process( "draw_site" ),
        _s( s ),
        _d( d )
    { };
    virtual ~draw_site( ) { };
protected:
    void inner_body( void ) 
    {
        while( 1 ) {
            cout << _s;
            hold( _d );
        }
    }
    const char _s;
    const double _d;
};

class simulation_main : public process {
public:
    simulation_main( ) :
        process( "simulation_main" )
    { 

    };
    virtual ~simulation_main( ) { };
protected:
    void inner_body( void )
    {
        handle<draw_site> p1( new draw_site( 'O', 2.0 ) );
        handle<draw_site> p2( new draw_site( '.', 2.0 ) );
        handle<draw_site> p3( new draw_site( '\n', 9.0 ) );
        p1->activateAfter( current( ) );
        p2->activateAt( 1.0 );
        p3->activateAt( 8.5 );
        hold( 62.6 );
        p1->cancel( );
        p2->cancel( );
        p3->cancel( );
    };
};

int main( void )
{
    simulation::instance()->begin_simulation( new sqsDll() );
    handle<simulation_main> m( new simulation_main( ) );
    m->activate( );
    simulation::instance()->run( );
    simulation::instance()->end_simulation( );
}
