/*****************************************************************************
 *
 * trace.hh
 *
 * Trace facilities for a discrete-event simulator
 *
 * Copyright (C) 2002 Moreno Marzolla, Paolo Palmerini
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

#ifndef _TRACE_HH
#define _TRACE_HH

#include <string>
#include "handle.hh"
#include "process.hh"

class trace {

public:

    enum trace_switch_t { trace_on = 0, trace_off };

    virtual ~trace( ) { };
    
    // 
    // Modifiers
    //
    enum trace_switch_t set_status( enum trace_switch_t t );
    enum trace_switch_t get_status( void ) const { return _status; };

    // p terminates at time t
    void log_terminate( const handle_p& p, double t );

    // p cancels q
    void log_cancel( const handle_p& p, const handle_p& q );

    // p passivates
    void log_passivate( const handle_p& p );

    // p holds for time dt
    void log_hold( const handle_p& p, double dt );

    //
    // Activation/Reactivation
    //

    // p (re)activates q
    void log_activate( const handle_p& p, const handle_p& q );
    void log_reactivate( const handle_p& p, const handle_p& q );

    // p (re)activates q at t
    void log_activateAt( const handle_p& p, const handle_p& q, double t );
    void log_reactivateAt( const handle_p& p, const handle_p& q, double t );

    // p (re)activates q after dt from now
    void log_activateDelay( const handle_p& p, const handle_p& q, double dt );
    void log_reactivateDelay( const handle_p& p, const handle_p& q, double dt );

    // p (re)activates q after r
    void log_activateAfter( const handle_p& p, const handle_p& q, const handle_p& r );
    void log_reactivateAfter( const handle_p& p, const handle_p& q, const handle_p& r ); 

    // p (re)activates q before r
    void log_activateBefore( const handle_p& p, const handle_p& q, const handle_p& r );
    void log_reactivateBefore( const handle_p& p, const handle_p& q, const handle_p& r );

    // Process destruction
    void log_destroy( const handle_p& p );

    //
    // Accessors
    //
    static trace* instance( void );

protected:

    trace( );

    void log( const handle_p& originator, 
              const char* fmt, ... );

    enum trace_switch_t _status;
    static trace*	_instance;
    double _time;
};

#endif
