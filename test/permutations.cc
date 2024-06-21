/****************************************************************************
 *
 * permutations.cc
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
 * Display Permutations. Adapted from
 * "A Portable C++ Library for Coroutine Sequencing", by Keld Helsgaun
 */

#include "coroutine.hh" 
#include <cassert>
#include <cstdlib>
#include <iostream>

using namespace std;

class Permuter : public coroutine 
{ 
public: 
    int *p, More; 

    Permuter( int n ) : 
        coroutine( ), 
        N( n ) 
    { 
        p = new int[N+1]; 
    } 
protected:

    const int N;

    void Permute( int k ) 
    { 
        if ( k == 1 ) 
            detach( ); 
        else { 
            Permute( k-1 ); 
            for ( int i = 1; i < k; i++ ) { 
                int q = p[i]; 
                p[i] = p[k]; 
                p[k] = q; 
                Permute( k-1 ); 
                q = p[i]; 
                p[i] = p[k]; 
                p[k] = q; 
            } 
        } 
    } 

    void main( void ) 
    { 
        for ( int i = 1; i <= N; i++ ) 
            p[i] = i; 
        More = 1; 
        Permute( N ); 
        More = 0; 
    } 
};

unsigned int factorial( int n )
{
    if ( n > 0 )
        return n*factorial( n-1 );
    else
        return 1;
};

void PrintPermutations( int n ) 
{ 
    Permuter *P = new Permuter( n ); 
    unsigned int n_permutations = 0;
    P->call( );
    while ( P->More ) { 
        for (int i = 1; i <= n; i++) 
            cout << P->p[i] << " "; cout << endl; 
        n_permutations++;
        P->resume( );
    } 
    //
    // Check if the number of computer permutations is correct
    //
    assert( n_permutations == factorial(n) );
};

int main( void )
{
    PrintPermutations( 5 );
}
