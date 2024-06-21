/*****************************************************************************
 *
 * coroutine.cc
 *
 * Coroutine implementation for a discrete-event simulator. 
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

/*
 * The following implementation of coroutines is strongly based on that on
 * "A Portable C++ Library for Coroutine Sequencing", by Keld Helsgaun
 * (keld@ruc.dk) Department of Computer Science Roskilde University,
 * DK-4000 Roskilde, Denmark
 * 
 * I modified the original implementation becahse I make use of
 * the makecontext/swapcontext system calls.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#include "coroutine.hh"
#include <iostream>
#include <cstdlib>
#include <cassert>
extern "C" {
#include <unistd.h>
}

using namespace std;

size_t coroutine::cStackSize = 16*(size_t)sysconf( _SC_PAGESIZE );
unsigned long coroutine::_num_ctx_switch = 0;

/*
 * This is the main coroutine. 
 */
class main_coroutine : public coroutine {
public:
    main_coroutine( void ) : coroutine( ) { };
    virtual ~main_coroutine( ) { };
protected:
    void main( void ) { };
} main_component;

coroutine* coroutine::_current_coroutine = &main_component;

#if COROUTINE_METHOD == mcsc

//////////////////////////////////////////////////////////////////////////////
//
// Part specific for the makecontext/swapcontext based switching method
//
//////////////////////////////////////////////////////////////////////////////

/*
 * Function used by makecontext as a "trampoline" to call the main method of 
 * a coroutine.
 */
void cor_trampoline( coroutine* c )
{
    c->main( );
    c->_cor_terminated = true;
    c->detach();
}

coroutine::coroutine( ) :
    _context    ( new ucontext_t ),
    _cor_terminated( false ),
    _caller     ( 0 ),
    _callee     ( 0 )
{
    getcontext( _context );
    _context->uc_flags          = 0;
    _context->uc_link           = 0;
    _context->uc_stack.ss_sp    = (char*)malloc( cStackSize );
    _context->uc_stack.ss_size  = cStackSize;
    makecontext( _context, (void(*)())cor_trampoline, 1, this );
}

coroutine::~coroutine( )
{
    free( _context->uc_stack.ss_sp );
    free( _context );
    assert( 0 == _caller );
    assert( 0 == _callee );
}

bool coroutine::terminated( void ) const
{
    return _cor_terminated;
}

void coroutine::enter( void )
{
    ucontext_t *old_context = _current_coroutine->_context;
    assert( 0 != old_context );
    assert( _context != old_context );
    _current_coroutine = this;
    _num_ctx_switch++;
    if ( -1 == swapcontext( old_context, _context ) ) {
        cerr << "FATAL: swapContext returned -1" << endl;
        abort( );
    };
}

#elif COROUTINE_METHOD == sjlj_copy

//////////////////////////////////////////////////////////////////////////////
//
// Part specific for the setjmp/longjmp based switching method
//
//////////////////////////////////////////////////////////////////////////////

#include <cstring>
/*
 * Taken from Helsgaun. Renamed some variables
 */

char* _stackBottom = 0;

coroutine::coroutine( ) :
    _stackBuffer( 0 ),
    _low        ( 0 ),
    _high       ( 0 ),
    _bufferSize ( 0 ),
    _caller     ( 0 ),
    _callee     ( 0 )
{
    char dummy;
    //
    // FIXME: I'm not totally sure wether this is the correct way to 
    // initialize _stackBottom. Helsgaun uses a marco "initSequencing"
    // which must be called when initializing the coroutine sequencing,
    // which defines a local dummy variable on the stack and initializes
    // _stackBottom to the address of such dummy variable. 
    //
    if ( !_stackBottom )
        _stackBottom = &dummy;

    if ( _stackBottom )
        if (&dummy < _stackBottom ? 
            &dummy <= (char*) this && (char*) this <= _stackBottom :
            &dummy >= (char*) this && (char*) this >= _stackBottom) {
            cerr << "FATAL: Attempt to allocate a Coroutine on the stack" 
                 << endl;
            abort( );
        }
}

coroutine::~coroutine( )
{
    delete _stackBuffer;
    _bufferSize = 0;
    assert( 0 == _caller );
    assert( 0 == _callee );
}

void coroutine::enter( void ) 
{
    if ( ! _current_coroutine->terminated() ) {
        _current_coroutine->storeStack( );
        if ( setjmp(_current_coroutine->_envir ) )
            return; 
    }
    _current_coroutine = this;
    if ( !_stackBuffer ) {
        main( );
        delete _current_coroutine->_stackBuffer;
        _current_coroutine->_stackBuffer = 0;
        detach( );
    }
    restoreStack( );
}

void coroutine::storeStack( void )
{
    if ( !_low ) {
        if ( !_stackBottom ) {
            cerr << "FATAL: StackBottom is not initialized" << endl;
            abort( );
        }
        _low = _high = _stackBottom;
    }
    char X;
    if ( &X > _stackBottom ) 
        _high = &X; 
    else 
        _low = &X;
    if ( _high - _low > _bufferSize ) {
        delete _stackBuffer;
        _bufferSize = _high - _low;
        if ( ! ( _stackBuffer = new char[ _bufferSize ] ) ) {
            cerr << "FATAL: No more space available" << endl;
            abort( );
        }
    }
    assert( 0 != _stackBuffer );
    memcpy( _stackBuffer, _low, _high - _low);
}

void coroutine::restoreStack( void )
{
    char X;
    if (&X >= _low && &X <= _high) 
        restoreStack( );
    _current_coroutine = this;
    memcpy( _low, _stackBuffer, _high - _low);
    longjmp(_current_coroutine->_envir, 1);
}

bool coroutine::terminated( void ) const
{
    return ( !_stackBuffer && _bufferSize );
}

#else

#error Unknown context switching method

#endif

//////////////////////////////////////////////////////////////////////////////
//
// Common part
//
//////////////////////////////////////////////////////////////////////////////

void coroutine::resume( void )
{ 
    coroutine* toResume = this;
    if ( toResume == coroutine::_current_coroutine ) 
        return;
    if ( toResume->terminated() ) {
        cerr << "FATAL: Cannot resume a terminated coroutine" << endl;
        abort( );
    }
    if ( toResume->_caller ) {
        cerr << "FATAL: Cannot resume an attached coroutine" << endl;
        abort( );
    }
    while ( toResume->_callee )
        toResume = toResume->_callee;
    toResume->enter( );
};

void coroutine::call( void )
{
    coroutine* toCall = this;
    if ( toCall->terminated() ) {
        cerr << "FATAL: Cannot call a terminated coroutine" << endl;
        abort( );
    }
    if ( toCall->_caller ) {
        cerr << "FATAL: Cannot call an attached coroutine" << endl;
        abort( );
    }
    coroutine::_current_coroutine->_callee = toCall;
    toCall->_caller = coroutine::_current_coroutine;
    while ( toCall->_callee )
        toCall = toCall->_callee;
    if ( toCall == coroutine::_current_coroutine ) {
        cerr << "FATAL: Cannot call an operating coroutine" << endl;
        abort( );
    }
    toCall->enter( );
}

void coroutine::detach( void )
{
    coroutine *next = _caller;
    if ( 0 != next ) 
        _caller = next->_callee = 0;
    else {
        next = &main_component; 
        while ( 0 != next->_callee)
            next = next->_callee;
    }
    assert( 0 != next );

    //
    // FIXME: Is this always the case?
    //
    assert( _current_coroutine == this );
    next->enter( );
};

