/****************************************************************************
 *
 * foo.cc
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

#include "cppsim.hh"
#include <string>
#include <list>

class myevent;
class mydelay;

class myproc : public process {
public:
    myproc( const string& name ) :
        process( name ) { };
    virtual ~myproc( ) { };
    list< myevent* > _events;
    void hold( double dt ) {
        process::hold( dt );
    };
protected:
    void inner_body( void );
};

class myevent {
public:
    virtual ~myevent( ) { };
    virtual void execute( void ) = 0;
protected:
    myevent( myproc* p ) : _p( p ) { };
    myproc* _p;
};

class mydelay : public myevent {
public:
    mydelay( myproc* p, double d ) :
        myevent( p ),
        _d( d )
    { };
    virtual ~mydelay( ) { };
    void execute( void ) {
        _p->hold( _d );
    }
protected:
    double _d;
};

void myproc::inner_body( void )
{
    list< myevent* >::iterator it;
    for ( it = _events.begin(); it != _events.end(); it++ ) {
        (*it)->execute( );
    }
}

int main( void )
{
    process::getLogger()->setLevel( 0 );
    simulation::instance()->begin_simulation( new sqsDll() );

    handle<myproc> p( new myproc( "myproc" ) );
    for ( int i=0; i<10; i++ )
        p->_events.push_back( new mydelay( p, 10 ) );
    p->activate();
    simulation::instance( )->run( );
    simulation::instance( )->end_simulation( );
    return 0;
}
