/*****************************************************************************
 *
 * process.cc
 *
 * Concurrent processes implementation for a discrete-event simulator
 *
 * Copyright (C) 2002, 2003, 2012 Moreno Marzolla
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

#ifndef CPPSIM_PROCESS_HH
#define CPPSIM_PROCESS_HH

#include <cstdlib>
#include <list>
#include <string>
#include <cstdarg>
#include "coroutine.hh"
#include "handle.hh"

class sqs;
class absEvNotice;
class process;
class logger;

typedef handle<process> handle_p;

class process : public coroutine, public shared {
public:

    process( const string& name );
    virtual ~process( );

    virtual void main( void );

    //
    // Modifiers
    //
    void activate( void );                      // put in front of the sqs
    void activateAfter( handle_p& q );          // activate AFTER q
    void activateBefore( handle_p& q );         // activate BEFORE q
    void activateAt( double t );                // activate at time t
    void activateDelay( double dt = 0.0 );      // activate with delay dt

    void reactivate( void );
    void reactivateAfter( handle_p& q );
    void reactivateBefore( handle_p& q );
    void reactivateAt( double t );
    void reactivateDelay( double dt = 0.0 );

    void cancel( void );                        // remove from the sqs
    
    //
    // Accessors (const)
    //
    bool        idle( void )       const { return ( 0 == _ev ); };
    bool        terminated( void ) const { return _terminated; };
    double      evTime( void )     const;
    int         id( void )         const { return _id; };
    const string& name( void )     const { return _name; };
    sqs*        getSqs( void )     const { return _s; };
    absEvNotice* notice( void )    const { return _ev; };
    static logger* getLogger( void ) 
    { 
	return logger::instance( "libcppsim::process" );
    };

protected:

    //
    // Modifiers
    //
    void hold( double dt );
    void passivate( void );
    void end_simulation( void );

    //
    // Accessors
    //
    handle_p& current( void ) const;
    double time( void ) const;

    virtual void inner_body( void ) = 0;

private:

    // Logging facility
    void message( const handle_p& p, const char* fmt, ... );

    const int           _id;
    const string        _name;
    bool                _terminated;
    sqs*                _s;  
    absEvNotice*        _ev;

    // Static members
    static int          _idCount;
};

#endif
