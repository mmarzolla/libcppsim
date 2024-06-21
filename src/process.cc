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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include "process.hh"
#include "sqs.hh"
#include "logger.hh"

// Time format string, used for logging
#define TIMEFMT "%.2f"

int process::_idCount = 0;

void process::main( void )
{
    inner_body( );
    message( this, "Terminates\n" );

    if ( 0 != _ev ) {
        getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
    _terminated = true;
}

process::process( const string& name ) : 
    coroutine   ( ),
    shared      ( ),
    _id         ( _idCount++ ), 
    _name       ( name ),
    _terminated ( false ),
    _s          ( simulation::instance( )->getSqs( ) ), 
    _ev         ( 0 )
{

}

process::~process( )
{
    if ( 0 != _ev ) {
	getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
}

//
// hold( t )
//
// Suspend the current process and reschedules it at time  "now + dt". 
//
// This function fails if the calling process is not the currently
// active one or if this process is not the currently active one,
// Also, this function fails if the current process does not have an
// event notice associated.  The latter condition is somewhat
// superfluous, since the currently active process _does_ have an
// event notice for the current simulated time.
//
// When calling this function, the current process is suspendproed,
// and control is passed back to the scheduler, which will select the
// next event notice from the sequencing set.
//
void process::hold( double dt ) 
{
    double now = time( );

    assert( this == current( ).rep( ) );
    assert( 0 != _ev );

    message( this, "Hold " TIMEFMT " until " TIMEFMT "\n", dt, evTime()+dt );

    getSqs( )->remove( _ev );
    delete _ev; _ev = 0;

    _ev = getSqs( )->insertAt( this, now + dt );
    detach( );
}

//
// passivate( )
//
// Removes the current process from the sequencing set, removing its
// event event notice. Control is immediately passed back to the
// scheduler, which will select the next event notice (if any). This
// function returns only if another process explicitly reactivates the
// calling process (by calling activate( ) or reactivate( ) for
// example).
//
// This function fails if the calling process is not the currently
// active one or if this process is not the currently active one,
// Also, this function fails if the current process does not have an
// event notice associated.  The latter condition is somewhat
// superfluous, since the currently active process _does_ have an
// event notice for the current simulated time.
//
void process::passivate( void ) 
{
    assert( this == current( ).rep( ) );
    assert( 0 != _ev );

    message( this, "Pass\n" );

    getSqs( )->remove( _ev );
    delete _ev; _ev = 0;

    detach( );
}

void process::activate( void )
{
    double now = time( );
    if ( 0 == _ev ) {
        message( current(), "Act %s [%d] now\n", name().c_str(), id() );
        _ev = getSqs( )->insertAt( this, now );
    }
}

void process::activateAt( double t )
{
    double now = time( );
    assert( t >= now );
    if ( 0 == _ev ) {
        message( current(), "Act %s [%d] at " TIMEFMT "\n", name().c_str(), id(), t );
        _ev = getSqs( )->insertAt( this, t );
    }
}

void process::activateBefore( handle_p& q )
{
    if ( 0 == _ev ) {
        message( current(), "Act %s [%d] before %s [%d] at " TIMEFMT "\n",
                 name().c_str(), id(), q->name().c_str(), q->id(), q->evTime() );
	_ev = getSqs( )->insert( this, q, pos_before );
    }
}

void process::activateAfter( handle_p& q )
{
    if ( 0 == _ev ) {
        message( current(), "Act %s [%d] after %s [%d] at " TIMEFMT "\n",
                 name().c_str(), id(), q->name().c_str(), q->id(), q->evTime() );
        _ev = getSqs( )->insert( this, q, pos_after );
    }
}

void process::activateDelay( double dt )
{
    double now = time( );
    if ( 0 == _ev ) {
        message( current(), "Act %s [%d] delay " TIMEFMT "\n", 
                 name().c_str(), id(), dt );
        _ev = getSqs( )->insertAt( this, now + dt );
    }
}

void process::reactivate( void )
{
    message( current(), "React %s [%d] now\n", name().c_str(), id() );
    double now = time( );
    if ( 0 != _ev ) {
        getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
    _ev = getSqs( )->insertAt( this, now );
}

void process::reactivateAt( double t )
{
    message( current(), "React %s [%d] at " TIMEFMT "\n", 
             name().c_str(), id(), t );
    if ( 0 != _ev ) {
        getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
    _ev = getSqs( )->insertAt( this, t );
}

void process::reactivateAfter( handle_p& q )
{
    message( current(), "React %s [%d] after %s [%d] at " TIMEFMT "\n",
             name().c_str(), id(), q->name().c_str(), q->id(), q->evTime() );
    if ( 0 != _ev ) {
        getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
    _ev = getSqs( )->insert( this, q, pos_after );
}

void process::reactivateBefore( handle_p& q )
{
    message( current(), "Act %s [%d] before %s [%d] at " TIMEFMT "\n",
             name().c_str(), id(), q->name().c_str(), q->id(), q->evTime() );
    if ( 0 != _ev ) {
        getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
    _ev = getSqs( )->insert( this, q, pos_before );
}

void process::reactivateDelay( double dt )
{
    message( current(), "React %s [%d] delay " TIMEFMT "\n", 
             name().c_str(), id(), dt );
    double now = time( );
    if ( 0 != _ev ) {
        getSqs( )->remove( _ev );
	delete _ev; _ev = 0;
    }
    _ev = getSqs( )->insertAt( this, now + dt );
}

handle_p& process::current( void ) const 
{ 
    return _s->current( );
}

void process::cancel( void )
{
    message( current(), "Canc %s [%d]\n", name().c_str(), id() );
    if ( 0 == _ev )
        return;

    handle_p cur = current( );
    getSqs( )->remove( _ev );
    delete _ev; _ev = 0;
    if ( getSqs( )->empty( ) )
        detach( );
    else
        if ( getSqs( )->firstProc( ).rep( ) != cur.rep( ) ) 
            detach( );
}

double process::time( void ) const
{
    return ( getSqs( )->empty( ) ? 0.0 : current( )->evTime( ) );
}

double process::evTime( void ) const
{
    assert( 0 != _ev );
    return _ev->evTime( );
}

void process::end_simulation( void ) 
{
    getSqs( )->clear( );
    detach( );
}

void process::message( const handle_p& p, const char* fmt, ... )
{
    double _time = 0.0;
    static char msgBuf[ 128 ];
    logger* _logger = getLogger();

    if ( ! _logger->enabled( 0 ) )
        return;

    if ( ! p.isNull( ) && p->notice( ) )
	_time = p->evTime( );

    _logger->message( 0, 
                      "%10.2f / %4d/%15s / ", 
                      _time,
                      ( p.isNull( ) ? -1 : p->id() ),
                      ( p.isNull( ) ? "Simulation Main" : p->name().c_str() ) );
    va_list ap;
    va_start( ap, fmt );
    _logger->vmessage( 0, fmt, ap );
    va_end( ap );
}
