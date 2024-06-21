/*****************************************************************************
 *
 * trace.cc -- tracing facilities for a discrete-event simulator
 *
 * Copyright (C) 2002, 2024 Moreno Marzolla, Paolo Palmerini
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

#include "process.hh"
#include "trace.hh"
#include "sqs.hh"
#include "handle.hh"

using namespace std;

trace* trace::_instance = 0;

// Time format string
#define TIMEFMT "%.2f"

trace::trace( ):
    _status( trace_off ),
    _time( 0.0 )
{

}

enum trace::trace_switch_t trace::set_status( enum trace::trace_switch_t t )
{
    enum trace::trace_switch_t old = _status;
    _status = t;
    if ( trace::trace_on == t )
        printf("%10s / %20s / %s\n", "Time", "Process ID", "Messages" );
    else
        printf("Trace stops\n");

    return old;
}

trace* trace::instance( void )
{
    if ( 0 == _instance )
	_instance = new trace( );
    return _instance;
}

void trace::log_terminate( const handle_p& p, double t )
{
    if ( _status == trace_on )
        log( p, "Terminates" );
}

void trace::log_cancel( const handle_p& p, const handle_p& q )
{
    if ( _status == trace_on )
        log( p, "Canc %s [%d]", q->name().c_str(), q->id() );
}

void trace::log_passivate( const handle_p& p )
{
    if ( _status == trace_on )
	log( p, "Pass" );
}

void trace::log_hold( const handle_p& p, double dt )
{
    if ( _status == trace_on )
        log( p, "Hold " TIMEFMT " until " TIMEFMT, dt, p->evTime()+dt );
}

//
// Activates
//
void trace::log_activate( const handle_p& p, const handle_p& q )
{
    if ( _status == trace_on )
        log( p, "Act %s [%d] now", q->name().c_str(), q->id() );
}

void trace::log_activateAt( const handle_p& p, const handle_p& q, double t )
{
    if ( _status == trace_on )
        log( p, "Act %s [%d] at " TIMEFMT, q->name().c_str(), q->id(), t );
}

void trace::log_activateDelay( const handle_p& p, const handle_p& q, double dt )
{
    if ( _status == trace_on )
        log( p, "Act %s [%d] delay " TIMEFMT, q->name().c_str(), q->id(), dt );
}

void trace::log_activateAfter( const handle_p& p, const handle_p& q, const handle_p& r )
{
    if ( _status == trace_on ) {
        log( p, "Act %s [%d] after %s [%d] at " TIMEFMT,
             q->name().c_str(), q->id(), r->name().c_str(), r->id(), r->evTime() );
    }
}

void trace::log_activateBefore( const handle_p& p, const handle_p& q, const handle_p& r )
{
    if ( _status == trace_on ) {
        log( p, "Act %s [%d] before %s [%d] at " TIMEFMT,
             q->name().c_str(), q->id(), r->name().c_str(), r->id(), r->evTime() );
    }
}

//
// Reactivate
//
//
// Activates
//
void trace::log_reactivate( const handle_p& p, const handle_p& q )
{
    if ( _status == trace_on )
        log( p, "React %s [%d] now", q->name().c_str(), q->id() );
}

void trace::log_reactivateAt( const handle_p& p, const handle_p& q, double t )
{
    if ( _status == trace_on )
        log( p, "React %s [%d] at " TIMEFMT, q->name().c_str(), q->id(), t );
}

void trace::log_reactivateDelay( const handle_p& p, const handle_p& q, double dt )
{
    if ( _status == trace_on )
        log( p, "React %s [%d] delay " TIMEFMT, q->name().c_str(), q->id(), dt );
}

void trace::log_reactivateAfter( const handle_p& p, const handle_p& q, const handle_p& r )
{
    if ( _status == trace_on ) {
        log( p, "React %s [%d] after %s [%d] at " TIMEFMT,
             q->name().c_str(), q->id(), r->name().c_str(), r->id(), r->evTime() );
    }
}

void trace::log_reactivateBefore( const handle_p& p, const handle_p& q, const handle_p& r )
{
    if ( _status == trace_on ) {
        log( p, "Act %s [%d] before %s [%d] at " TIMEFMT,
             q->name().c_str(), q->id(), r->name().c_str(), r->id(), r->evTime() );
    }
}

//
// FIXME: This is not used. Apparently there is a problem with a process
// being destroyed multiple times if this function is called in the destructor
//
void trace::log_destroy( const handle_p& p )
{
    if ( _status == trace_on )
        log( p, "Destroyed" );
}

void trace::log( const handle_p& p, const char* fmt, ... )
{
    if ( ! p.isNull( ) && p->notice( ) )
	_time = p->evTime( );

    printf("%10.2f / %4d/%15s / ",
           _time,
           ( p.isNull( ) ? -1 : p->id() ),
           ( p.isNull( ) ? "Simulation Main" : p->name().c_str() ) );

    va_list ap;
    va_start( ap, fmt );
    vprintf( fmt, ap );
    va_end( ap );
    printf("\n");
}
