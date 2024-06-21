/*****************************************************************************
 *
 * sqsPrio.hh
 *
 * Implementation of a Sequencing Set using a priority queue
 *
 * Copyright (C) 2002, 2003, 2013 Moreno Marzolla.
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

#ifndef _SQSPRIO_HH
#define _SQSPRIO_HH

#include <list>
#include <map>
#include <cstdlib>
#include "process.hh"
#include "sqs.hh"

class prioEvNotice;

class sqsPrio : public sqs {
public:

    // Typedefs
    typedef list< prioEvNotice* >     bin_t;
    typedef map< double, bin_t >      sqs_t;
    typedef prioEvNotice              evn_t;

    sqsPrio( );
    virtual ~sqsPrio( );

    // Modifiers
    absEvNotice* insertAt( const handle_p& p, double t );
    absEvNotice* insert( const handle_p& p, 
                         const handle_p& q, 
                         enum position_t where );
    void remove( absEvNotice* ev );
    void clear( void );

    // Accessors
    absEvNotice* first( void ) const;
    void dump_sqs( void ) const;
    void report( void ) const;
    bool empty( void ) const { return ( 0 == _sqs_len ); };
    unsigned int size( void ) const { return _sqs_len; };

protected:

    sqs_t _sqs;
    unsigned long _sqs_len;
};

class prioEvNotice : public absEvNotice {
    friend class sqsPrio; 
public:
    virtual ~prioEvNotice( ) { };
    
protected:
    prioEvNotice( double t, const handle_p& p ) : 
	absEvNotice( t, p ) { };

    sqsPrio::bin_t::iterator _pos_bin; // Position in the bin
    sqsPrio::sqs_t::iterator _pos_sqs; // Position in the sqs
};

#endif
