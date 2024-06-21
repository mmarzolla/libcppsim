/*****************************************************************************
 *
 * coroutine.hh
 *
 * This file is part of cppsim
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

#ifndef CPPSIM_COROUTINE_HH
#define CPPSIM_COROUTINE_HH

#if HAVE_CONFIG_H
#include <config.h>
#endif

#if COROUTINE_METHOD == mcsc    // makecontext/swapcontext

extern "C" {
#include <ucontext.h>
}

#else                           // setjmp/longjmp

extern "C" {
#include <setjmp.h>
}
#include <cstdlib>

#endif                          // End alternative part

class coroutine
{

#if CONTEXT_METHOD == mcsc      // makecontext/setcontext
    friend void cor_trampoline( coroutine * c );
#endif                          // End alternative part

public:
    virtual ~coroutine ();
    
    // Accessors
    inline bool terminated( void ) const;
    
    // Modifiers
    void resume( void );
    void call( void );
    
    static size_t cStackSize;
protected:
    coroutine( );
    void detach( void );
    virtual void main( void ) = 0;              // Coroutine body
    
    static unsigned long _num_ctx_switch;	// Number of context switches
private:
    inline void enter( void );

#if COROUTINE_METHOD == mcsc            // makecontext/setcontext
    ucontext_t  *_context;
    bool        _cor_terminated;
#else                                   // setjmp/longjmp, copy
    inline void storeStack( void );
    inline void restoreStack( void );

    jmp_buf     _envir;
    char        *_stackBuffer, *_low, *_high;
    size_t      _bufferSize;
#endif

    coroutine   *_caller;
    coroutine   *_callee;
    static coroutine *_current_coroutine;
};

#endif
