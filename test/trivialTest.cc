/****************************************************************************
 *
 * trivialTest.cc
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

#include <iostream>
#include "cppsim.hh"

class job : public process {
public:
    job( const string& name ) :
        process( name )
    { 
        // cout << "Created job '" << name << "' " << id() << endl;
    };
    virtual ~job( )
    { 
        // cout << "Destroyed job '" << name() << "' " << id() << endl;
    };
protected:
    void inner_body( void ) {
        hold( 10 );
    };
};

class source : public process {
public:
    source( const string& name ) :
        process( name )
    { };
    virtual ~source( )
    { };
protected:
    void inner_body( void ) {
        handle<job> j;
        for ( int i=0; i<10; i++ ) {
            hold( 5 );
            j = new job( "job" );
            j->activateAfter( current() );
        }
        end_simulation( );
    };
};

int main( void )
{
    process::getLogger()->setLevel( 0 );
    simulation::instance()->begin_simulation( new sqsDll() );
    handle<source> src = new source( "source" );
    src->activate();
    simulation::instance()->run();
    simulation::instance()->end_simulation();
    cout << "Finished" << endl;
    return 0;
};
