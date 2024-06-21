/*****************************************************************************
 *
 * sqsPrio.cc
 *
 * Sequencing Set implementation based on a priority queue.
 *
 * Copyright (C) 2002, 2003, 2013 Moreno Marzolla
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

#include <iostream>
#include <cassert>
#include "handle.hh"
#include "sqsPrio.hh"
#include "process.hh"

sqs* sqsPrioBuilder( void ) 
{
    return new sqsPrio( );
}

sqsPrio::sqsPrio( ) : 
    sqs         ( ),
    _sqs_len    ( 0 )
{
  
}

sqsPrio::~sqsPrio( )
{

}

//
// Creates a new event notice for process p at time t. Process p is inserted
// _before_ any other process already scheduled for time t.
//
// Preconditions:
// - t must be greater or equal to the current simulation time;
// - p should _not_ already have an associated event notice;
//
absEvNotice* sqsPrio::insertAt( const handle_p& p, double t )
{
    assert( !p->terminated( ) );
    assert( 0 == p->notice( ) );

    prioEvNotice* ev( new prioEvNotice( t, p ) );
    sqs_t::iterator sqs_it = _sqs.find(t);

    if ( _sqs.end( ) == sqs_it ) {
	bin_t emptyBin;

        // Create a temporary sqs element (must be a pair)
        sqs_t::value_type tmp( t, emptyBin );

        // Insert the element into the map
        sqs_it = _sqs.insert( tmp ).first;

    }

    bin_t& theList = sqs_it->second;

    // theBin.push_front( ev );
    theList.push_front( ev );

    ev->_pos_bin = theList.begin( );
    ev->_pos_sqs = sqs_it;

    _insertions++;
    _sqs_len++;
    return ev;
}

//
// Schedules p immediately after or before q
//
absEvNotice* sqsPrio::insert( const handle_p& p, 
                              const handle_p& q, 
                              enum position_t where )
{
    prioEvNotice* ev_q = dynamic_cast< prioEvNotice* >( q->notice( ) );
    prioEvNotice* ev_p = dynamic_cast< prioEvNotice* >( p->notice( ) );

    assert( !p->terminated( ) );
    assert( !q->terminated( ) );
    assert( 0 == ev_p );
    assert( 0 != ev_q );

    bin_t::iterator bin_it( ev_q->_pos_bin );

    if ( where == pos_after ) 
        bin_it++;

    double t = ev_q->evTime( );
    ev_p = new prioEvNotice( t, p );
    bin_t& theList( _sqs[t] );

    ev_p->_pos_bin = theList.insert( bin_it, ev_p );
    ev_p->_pos_sqs = ev_q->_pos_sqs;

    _insertions++;
    _sqs_len++;
    return ev_p;
}

//
// Removes the event notice ev from the sequencing set.
//
void sqsPrio::remove( absEvNotice* ev )
{
    prioEvNotice* _prioqEv( dynamic_cast< prioEvNotice* >( ev ) );
    assert( 0 != _prioqEv );

    bin_t::iterator bin_it( _prioqEv->_pos_bin );
    sqs_t::iterator sqs_it( _prioqEv->_pos_sqs );

    sqs_t::mapped_type& _list( sqs_it->second );
    _list.erase( bin_it );

    if ( _list.empty( ) )
        _sqs.erase( sqs_it );

    _deletions++;
    assert( _sqs_len > 0 );
    _sqs_len--;
}

void sqsPrio::clear( void )
{
    _sqs.clear( );
    _sqs_len = 0;
}

void sqsPrio::dump_sqs( void ) const
{
    sqs_t::const_iterator _sqs_it;
    cout << "[SQS Head ";    
    for ( _sqs_it = _sqs.begin( ); _sqs_it != _sqs.end( ); _sqs_it ++ ) {
        bin_t::const_iterator _list_it;
        bin_t _list( _sqs_it->second );
        for ( _list_it = _list.begin( ); _list_it != _list.end( ); _list_it++ )
            cout << "(proc=" << (*_list_it)->_proc->id( )
                 << "/" << (*_list_it)->_proc->name( )
                 << " time=" << (*_list_it)->_time << ") ";
    }
    cout << " SQS Tail]" << endl;
}

absEvNotice* sqsPrio::first( void ) const
{
    if ( empty( ) ) {
        return 0;	
    } else {
        // Find the bin
        sqs_t::const_iterator _it( _sqs.begin( ) );
        assert( _it != _sqs.end( ) );
        
        // Get the list of events associated to the bin
        bin_t _list( _it->second );
        assert( !_list.empty() );

        // Get the first event notice of the list
        bin_t::const_iterator _it2( _list.begin( ) );
        assert( _it2 != _list.end( ) );
        
        return *_it2;
    }
}

void sqsPrio::report( void ) const
{
    cout << endl
	 << "Priority Queue SQS statistics:" << endl
	 << "  Num. of insertions..........." << _insertions << endl
	 << "  Num. of deletions............" << _deletions << endl
	 << endl;
}

