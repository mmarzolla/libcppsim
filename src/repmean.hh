/*****************************************************************************
 *
 * repmean.hh
 *
 * Estimation of the mean using independent replications, for terminating
 * simulations.
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

#ifndef _CPPSIM_REPMEAN_HH
#define _CPPSIM_REPMEAN_HH

#include <vector>
#include <string.h>
#include "statistic.hh"
#include "mean.hh"
#include "trremoval.hh"

class repmean : public statistic {
public:
    repmean( const string& name, 
             double confl, 
             trremoval* t = 0
             );
    virtual ~repmean( );
    void update( double v );    // Insert the new value into the list
    void reset( void );         // Starts a new replication
    void report( void ) const;
    confInt value( void );
protected:
    mean _m_all;        // Mean of independent replications
    trremoval* _t;      // Transient removal subroutine    
};

#endif
