/*****************************************************************************
 *
 * sqsCal.cc
 *
 * Calendar Queue-based sequencing set implementation
 *
 * Copyright (C) 2002 Moreno Marzolla
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
#include "sqsCal.hh"
#include "process.hh"

sqsCal::sqsCal( unsigned int nbins,     // Number of entries
                double bwidth           // Width of an entry
                ) :
    sqs( ),
    _sqs( nbins ),
    _nbins( nbins ),
    _bwidth( bwidth ),
    _t( 0.0 ),
    _sqs_len( 0 )
{

}

sqsCal::~sqsCal( )
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
absEvNotice* sqsCal::insertAt( const handle_p& p, double t )
{
    bin_t::iterator _sqs_it;

    assert( !p->terminated( ) );
    assert( 0 == p->notice( ) );

    unsigned int i = (unsigned int)( t / _bwidth ) % _nbins;

    for ( _sqs_it = _sqs[i].begin( ); _sqs_it != _sqs[i].end( ); _sqs_it++ ) {
	if ( (*_sqs_it)->_time >= t ) 
	    break;
    }
    
    calEvNotice* ev = new calEvNotice( t, p );
    
    ev->_pos = _sqs[i].insert( _sqs_it, ev );
    _insertions++;
    _sqs_len++;
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
absEvNotice* sqsCal::insert( const handle_p& p, 
                             const handle_p& q, 
                             enum position_t where )
{
    calEvNotice* ev_q = dynamic_cast<calEvNotice*>( q->notice( ) );
    calEvNotice* ev_p = dynamic_cast<calEvNotice*>( p->notice( ) );

    assert( !p->terminated( ) );
    assert( !q->terminated( ) );
    assert( 0 == ev_p );
    assert( 0 != ev_q );

    double t = ev_q->evTime( );
    unsigned int i = (unsigned int)( t / _bwidth ) % _nbins;

    bin_t::iterator _sqs_it( ev_q->_pos );

    assert( _sqs_it != _sqs[i].end( ) );

    if ( where == pos_after )
	_sqs_it++;

    ev_p = new calEvNotice( t, p );
    ev_p->_pos = _sqs[i].insert( _sqs_it, ev_p );
    _insertions++;
    _sqs_len++;
    return ev_p;
}

//
// Removes the event notice ev from the sequencing set.
//
void sqsCal::remove( absEvNotice* ev )
{
    calEvNotice* calEv = dynamic_cast<calEvNotice*>( ev );
    assert( 0 != calEv );

    bin_t::iterator _sqs_it( calEv->_pos );
    double t = ev->evTime( );
    unsigned int i = (unsigned int)( t / _bwidth ) % _nbins;

    _sqs[i].erase( _sqs_it );
    _deletions++;
    _sqs_len--;
}

void sqsCal::reset( void )
{

}

absEvNotice* sqsCal::first( void ) const
{
    if ( empty( ) )
	return (absEvNotice*)0;
    else {
        unsigned int i = (unsigned int)( _t / _bwidth ) % _nbins;
        while( 1 ) {
            calEvNotice* ev = _sqs[i].front( );
            if ( ev && ev->evTime( ) < _t + _bwidth )
                return ev;
            else {
                _t += _bwidth;
                i = (i+1) % _nbins;
            }
        }
    }
}

void sqsCal::dump_sqs( void ) const
{
    unsigned int i;
    cout << "[SQS Head]" << endl;    
    for ( i = 0; i < _nbins; i++ ) {
        cout << "Bin[" << i << "]=";
        
        bin_t::const_iterator _bin_it;
        for ( _bin_it = _sqs[i].begin( ); _bin_it != _sqs[i].end( ); _bin_it++ ) 
            cout << "(proc=" << (*_bin_it)->_proc->id( )
                 << "/" << (*_bin_it)->_proc->name( )
                 << " time=" << (*_bin_it)->_time << ") ";
        cout << endl;
    }
    cout << "[SQS Tail]" << endl;
}

void sqsCal::report( void ) const
{
    cout << endl
	 << "Calendar Queue SQS statistics:" << endl
	 << "  Num. of insertions..........." << _insertions << endl
	 << "  Num. of deletions............" << _deletions << endl
	 << endl;
}
