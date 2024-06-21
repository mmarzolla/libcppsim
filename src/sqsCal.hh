/*****************************************************************************
 *
 * sqsCal.hh
 *
 * Implementation of a Sequencing Set using calendar queues
 *
 * Copyright (C) 2002 Moreno Marzolla.
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

#ifndef _SQSCAL_HH
#define _SQSCAL_HH

extern "C" {
#include <stdlib.h>
}
#include <list>
#include <vector>
#include "handle.hh"
#include "process.hh"
#include "sqs.hh"

class calEvNotice;

class sqsCal : public sqs {
public:

    // Typedefs
    typedef list< calEvNotice* >        bin_t;
    typedef vector< bin_t >             sqs_t;
    typedef calEvNotice                 evn_t;

    sqsCal( unsigned int nbins, double bwidth );
    virtual ~sqsCal( );

    // Modifiers
    absEvNotice* insertAt( const handle_p& p, double t );
    absEvNotice* insert( const handle_p& p, const handle_p& q, enum position_t where );
    void remove( absEvNotice* ev );
    void reset( void );

    // Accessors
    absEvNotice* first( void )  const;
    void dump_sqs( void )       const;
    void report( void )         const;
    bool empty( void )          const { return ( 0 == _sqs_len ); };
    unsigned int size( void )   const { return _sqs_len; };

protected:

    sqs_t _sqs;
    unsigned int _nbins;        // Number of bins
    double _bwidth;             // Width of a bin
    mutable double _t;          // Current time
    unsigned long _sqs_len;     // Number of events into the SQS

};

class calEvNotice : public absEvNotice {
    friend class sqsCal; 
public:
    virtual ~calEvNotice( ) { };
    
protected:
    calEvNotice( double t, const handle_p& p ) : 
	absEvNotice( t, p ) { };

    sqsCal::bin_t::iterator _pos; // Position in the bin
};

#endif
