/*****************************************************************************
 *
 * mean.hh
 *
 * Compute the mean of a sequence of UNCORRELATED observations
 * x_1, x_2, \ldots x_N 
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

#ifndef _CPPSIM_MEAN_HH
#define _CPPSIM_MEAN_HH

#include <vector>
#include <stdexcept>
#include "statistic.hh"

class mean : public statistic {
public:
    mean( const string& name, double confl );
    virtual ~mean( );
    virtual void update( double v );
    virtual void reset( void );
    virtual void report( void ) const throw( runtime_error );
    virtual confInt value( void ) throw( runtime_error );

    // Accessors specific for this class

    // Returns the mean (average) of values
    double m( void ) const throw( runtime_error );        

    // Returns the variance
    double variance( void ) const throw( runtime_error ); 

protected:
    double _sum;        // Sum of observations
    double _sumsq;      // Sum of squared observations
};

#endif
