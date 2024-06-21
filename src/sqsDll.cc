/*****************************************************************************
 *
 * sqsDll.cc
 *
 * Simula-like doubly-linked list sequencing set.
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

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <iostream>

#ifdef __GNUC__
#if __GNUC__ < 3
#include <functional>
#else
#include <ext/functional>
using namespace __gnu_cxx;
#endif // __GNUC__ < 3
#endif // __GNUC__

#include <algorithm>
#include <fstream>
#include "handle.hh"
#include "sqsDll.hh"
#include "process.hh"

sqs* sqsDllBuilder( void ) 
{
    return new sqsDll( );
}

// #define DO_STATS

sqsDll::sqsDll( ) :
    sqs( )
#ifdef DO_STATS
    , os( "sqsDll.dump" )
#endif
{

}

sqsDll::~sqsDll( )
{

}

//
// Creates a new event notice for process p at time t. Process p is inserted
// _before_ any other process already scheduled for time t.
//
// Preconditions:
// - t must be greater or equal to the current simulation time;
// - p must _not_ already have an associated event notice;
//
// This routine is highly inefficient. The sequencing set is scanned linearly
//
absEvNotice* sqsDll::insertAt( const handle_p& p, double t )
{
    sqs_t::iterator _sqs_it;

    assert( !p->terminated( ) );
    assert( 0 == p->notice( ) );

    /*
     * We have to find the first position _sqs_it in the Sequencing Set
     * such that the event time is greater or equal than t. The easy
     * way to do that is the following:
     *
     * for ( _sqs_it = _sqs.begin( ); _sqs_it != _sqs.end( ); _sqs_it++ ) {
     *   if ( (*_sqs_it)->_time >= t ) 
     *     break;
     * }
     *
     * Using the <functional> classes, the above construct can be
     * coded as follows:
     */
    _sqs_it = find_if( _sqs.begin(), _sqs.end(), 
                       compose1( 
                                bind2nd( greater_equal<double>(), t ),
                                mem_fun( &absEvNotice::evTime )
                                )
                       );             

    assert( _sqs_it == _sqs.end() || (*_sqs_it)->evTime() >= t );
    dllEvNotice* ev = new dllEvNotice( t, p );
    
    ev->_pos = _sqs.insert( _sqs_it, ev );
    _insertions++;

#ifdef DO_STATS
    os << first()->evTime() << " " << size() << endl;
#endif

    return ev;
}

//
// Schedules p immediately after or before q
//
// Preconditions:
// - p and q must not be terminated processes;
// - p must _not_ already have an associated event notice;
// - q _must_ have an associated event notice.
//
absEvNotice* sqsDll::insert( const handle_p& p, 
                             const handle_p& q, 
                             enum position_t where )
{
    dllEvNotice* ev_q = dynamic_cast<dllEvNotice*>( q->notice( ) );
    dllEvNotice* ev_p = dynamic_cast<dllEvNotice*>( p->notice( ) );

    assert( !p->terminated( ) );
    assert( !q->terminated( ) );
    assert( 0 == ev_p );
    assert( 0 != ev_q );

    sqs_t::iterator _sqs_it( ev_q->_pos );

    assert( _sqs_it != _sqs.end( ) );

    if ( where == pos_after )
	_sqs_it++;

    double t = ev_q->evTime( );
    ev_p = new dllEvNotice( t, p );

    ev_p->_pos = _sqs.insert( _sqs_it, ev_p );
    _insertions++;

#ifdef DO_STATS
    os << first()->evTime() << " " << size() << endl;
#endif

    return ev_p;
}

//
// Removes the event notice ev from the sequencing set.
//
void sqsDll::remove( absEvNotice* ev )
{
#ifdef DO_STATS
    os << first()->evTime() << " ";
#endif

    dllEvNotice* dllEv = dynamic_cast<dllEvNotice*>( ev );
    assert( 0 != dllEv );

    sqs_t::iterator _sqs_it( dllEv->_pos );
    _sqs.erase( _sqs_it );
    _deletions++;

#ifdef DO_STATS
    os << size() << endl;
#endif
}

void sqsDll::clear( void )
{
    _sqs.clear( );
}

absEvNotice* sqsDll::first( void ) const
{
    return ( empty() ? 0 : _sqs.front( ) );
}

void sqsDll::dump_sqs( void ) const
{
    sqs_t::const_iterator _sqs_it;
    cout << "[SQS Head ";    
    for ( _sqs_it = _sqs.begin( ); _sqs_it != _sqs.end( ); _sqs_it ++ ) 
	cout << "(proc=" << (*_sqs_it)->_proc->id( )
	     << "/" << (*_sqs_it)->_proc->name( )
	     << " time=" << (*_sqs_it)->_time << ") ";
    cout << " SQS Tail]" << endl;
}

void sqsDll::report( void ) const
{
    cout << endl
	 << "Doubly-linked list SQS statistics:" << endl
	 << "  Num. of insertions..........." << _insertions << endl
	 << "  Num. of deletions............" << _deletions << endl
	 << endl;
}
