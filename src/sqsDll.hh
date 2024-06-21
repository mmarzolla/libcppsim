/*****************************************************************************
 *
 * sqsDll.hh
 *
 * Implementation of a Sequencing Set using a Simula-like doubly-linked list.
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

#ifndef SQSDLL_HH
#define SQSDLL_HH

#include <cstdlib>
#include <list>
#include <fstream>
#include "process.hh"
#include "sqs.hh"

class dllEvNotice;

class sqsDll : public sqs {
public:

    // Typedefs
    typedef list< dllEvNotice* > sqs_t;
    typedef dllEvNotice          evn_t;

    sqsDll( );
    virtual ~sqsDll( );

    // Modifiers
    absEvNotice* insertAt( const handle_p& p, double t );
    absEvNotice* insert( const handle_p& p, const handle_p& q, enum position_t where );
    void remove( absEvNotice* ev );
    void clear( void );

    // Accessors
    absEvNotice* first( void )  const;
    void dump_sqs( void )       const;
    void report( void )         const;
    bool empty( void )          const { return _sqs.empty( ); };
    unsigned int size( void )   const { return _sqs.size( ); };

protected:

    sqs_t _sqs;
    ofstream os;

};

class dllEvNotice : public absEvNotice {
    friend class sqsDll;
public:
    virtual ~dllEvNotice( ) { };

protected:
    dllEvNotice( double t, const handle_p& p ) : 
	absEvNotice( t, p ) { };

    sqsDll::sqs_t::iterator _pos;
};

#endif
