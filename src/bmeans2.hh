/****************************************************************************
 *
 * bmeans2.hh
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

#ifndef LIBCPPSIM_BMEANS2_HH
#define LIBCPPSIM_BMEANS2_HH

#include <vector>
#include <string>
#include <stdexcept>
#include "statistic.hh"
#include "trremoval.hh"

class bmeans2 : public statistic {
public:
    bmeans2( const string& name, 
             double confp,       // The probability for the confidence interval
             trremoval* t = 0 );
    virtual ~bmeans2( );
    void update( double v );    // Insert the new value into the list
    void reset( void );       
    void report( void ) const throw( runtime_error );
    confInt value( void ) throw( runtime_error );
protected:
    trremoval* _t;
};

#endif
