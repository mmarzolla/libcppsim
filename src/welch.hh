/*****************************************************************************
 *
 * welch.hh
 *
 * This file is part of cppsim
 *
 * Copyright (C) 2003, 2024 Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

#ifndef CPPSIM_WELCH_HH
#define CPPSIM_WELCH_HH

#include <vector>
#include <string>
#include "statistic.hh"

class welch : public statistic {
public:
    welch( const string& name,
           double confl,                // Confidence level
           unsigned int w               // half window size
           );
    virtual ~welch( );
    virtual void update( double v );    // Insert the new value into the list
    virtual void reset( void );         // Starts a new replication
    virtual void report( void ) const;
protected:
    virtual confInt value( void );      // Triggers abort()

    vector< vector<double> > _ob;       // Vectors of observations
    const unsigned int _wsize;          // Half window size
};

#endif
