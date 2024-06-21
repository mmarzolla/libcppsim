/****************************************************************************
 *
 * cor1.cc
 *
 * This file is part of cppsim
 *
 * Copyright (C) 2003 Moreno Marzolla
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 ****************************************************************************/

/*
 * cor1. Basic coroutine scheduling test. There are three coroutines,
 * A, B, and C. They activate themselves (via resume() calls) in the
 * following order:
 *
 * A->B->C->A->C->B
 *
 * This is also the output of thie program.
 */
#include <iostream>
#include "coroutine.hh"

using namespace std;

class cor_A : public coroutine {
public:
    cor_A( void ) : coroutine( ) { };
    virtual ~cor_A( ) { };
protected:
    void main( void );
};

class cor_B : public coroutine {
public:
    cor_B( void ) : coroutine( ) { };
    virtual ~cor_B( ) { };
protected:
    void main( void );
};

class cor_C : public coroutine {
public:
    cor_C( void ) : coroutine( ) { };
    virtual ~cor_C( ) { };
protected:
    void main( void );
};

cor_A *A;
cor_B *B;
cor_C *C;

void cor_A::main( void ) 
{
    cout << "A->";
    B->resume( );
    cout << "A->";
    C->resume( );
};

void cor_B::main( void )
{
    cout << "B->";
    C->resume( );
    cout << "B" << endl;
};

void cor_C::main( void )
{
    cout << "C->";
    A->resume( );
    cout << "C->";
    B->resume( );
};

int main( void )
{
    A = new cor_A();
    B = new cor_B();
    C = new cor_C();

    A->resume( );

    return 0;
}
