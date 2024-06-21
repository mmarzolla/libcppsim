/*****************************************************************************
 *
 * sqs.hh
 *
 * This file is part of libcppsim.
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

#ifndef SQS_HH
#define SQS_HH

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include "process.hh"
#include "simulation.hh"

class logger;

enum position_t { pos_after=0, pos_before };
enum sched_t    { SQSDLL = 0, SQSPRIO, SQSCAL };

class absEvNotice {
public:
    virtual ~absEvNotice( ) { };

    // Accessors

    double evTime( void ) const { return _time; };
    handle_p& proc( void )      { return _proc; };

protected:

    absEvNotice( double t, const handle_p& p ) :
        _time( t ), _proc( p ) 
    { 
        assert( !p.isNull( ) );
    };

    const double        _time;  // Event Time
    handle_p            _proc;  // Process associated to this event notice
};

class sqs : public coroutine {
public:

    virtual ~sqs( ) { };

    // Modifiers
    virtual absEvNotice* insertAt( const handle_p& p, double t ) = 0;
    virtual absEvNotice* insert( const handle_p& p, const handle_p& q, enum position_t where ) = 0;
    virtual void remove( absEvNotice* ev ) = 0;
    virtual void clear( void ) = 0;     // Empty the SQS

    // Accessors (const)
    virtual absEvNotice* first( void )  const = 0;
    handle_p& firstProc( void )         const;
    virtual void dump_sqs( void )       const = 0;
    virtual void report( void )         const = 0;
    virtual unsigned int size( void )   const = 0;
    virtual bool empty( void )          const = 0;

    // Accessors
    handle_p& current( void )           { return _current;      };
    logger* getLogger( void ) const     { return _logger;       };
    unsigned long insertions( void ) const      { return _insertions;   };
    unsigned long deletions( void ) const       { return _deletions;    };
    unsigned long processed( void ) const       { return _processed;    };

protected:
    sqs( );  

    // Runs the simulation
    void main( void );

    unsigned long _insertions;  // Number of insertions into the SQS
    unsigned long _deletions;   // Number of deletions from the SQS
    unsigned long _processed;   // Number of processed events

    handle_p    _current;       // current process
    static unsigned int _id;    // used for the name of the logger
    logger*     _logger;        // logger for the sqs length
};

//
// Utility functions, to build the proper scehduler
//
typedef sqs*(*sqsBuilder_t)( void );

sqs* sqsDllBuilder( void );
sqs* sqsPrioBuilder( void );

extern sqsBuilder_t builders[2];

sqs* makeSqs( sched_t s );

#endif
