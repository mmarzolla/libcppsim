/****************************************************************************
 *
 * dicegame.cc
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
 * Simple Dice Game. Adapted from
 * "A Portable C++ Library for Coroutine Sequencing", by Keld Helsgaun
 *
 * This program simulates a dice game between N coroutines. Each coroutine
 * in turn draws the dice and accumulates the result. The first  one
 * which accumulates 100 points wins.
 */
#include <cstdlib> 
#include <iostream> 
#include "coroutine.hh" 

using namespace std;

class Player : public coroutine { 
public: 
    Player *Next; 
    Player( int id ) : coroutine( ), Id(id) {} 
protected:
    void main( void ) 
    { 
        int Sum = 0; 
        do {
            int d = rand( ) % 6 + 1;
            Sum += d;
            cout << "Player " << Id << " got " << d 
                 << " (sum is now " << Sum << ")" << endl;
            if ( Sum < 100 )
                Next->resume( );
        } while ( Sum < 100 );
        cout << "The winner is player " << Id << endl; 
    } 

    int Id; 
}; 

void DiceGame( int Players ) 
{ 
    Player *FirstPlayer = new Player(1), *p = FirstPlayer; 
    for (int i = 2; i <= Players; i++, p = p->Next) 
        p->Next = new Player(i); 
    p->Next = FirstPlayer; 
    FirstPlayer->resume( );
} 

int main( )
{
    DiceGame( 5 );
}
