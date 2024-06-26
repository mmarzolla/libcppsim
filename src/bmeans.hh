/*****************************************************************************
 *
 * bmeans.hh -- Transient removal using batch means
 *
 * Copyright (C) 2002, 2003, 2024 Moreno Marzolla
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 *****************************************************************************/

#ifndef CPPSIM_BMEANS_HH
#define CPPSIM_BMEANS_HH

#include <vector>
#include <string>
#include <stdexcept>
#include "statistic.hh"
#include "trremoval.hh"


class bmeans : public statistic {
public:
    bmeans( const string& name,
	    double confp,       // The probability for the confidence interval
	    unsigned int bsize, // Batch Size
            trremoval* t = 0 );
    virtual ~bmeans( );
    void update( double v );    // Insert the new value into the list
    void reset( void );
    void report( void ) const;
    confInt value( void );

protected:

    confInt precompute( int bsize, double p,
			double& variance, double& voc ) const;

    unsigned int  _bsize;       // Batch size
    trremoval* _t;
    vector<double> _sum;
};

#endif
