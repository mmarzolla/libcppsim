/****************************************************************************
 *
 * simulation.cc
 *
 * This file is part of libcppsim
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

#ifndef _CPPSIM_SIMULATION_HH
#define _CPPSIM_SIMULATION_HH

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <list>

class sqs;
using namespace std;

class simulation  {
public:
    virtual ~simulation( ) { };

    //
    // Modifiers
    //

    // Adds a new sqs in front of the list. This means starting a new
    // simulation level (all processes from now on will be bound to the
    // newly created sqs). 's' must not be deleted directly, but only
    // via the simulation_end() method
    void begin_simulation( sqs* s );
    
    void end_simulation( );

    void run( void );

    // Returns the number of processed event notices
    unsigned long events( void ) const;

    static simulation* instance( void );

    //
    // Accessors
    //
    sqs* getSqs( void );

protected:
    simulation( ) { };

    list< sqs* > _sqs;
    static simulation* _instance;
};

#endif
