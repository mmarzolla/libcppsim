/*****************************************************************************
 *
 * mean.hh --  Compute the mean of a sequence of UNCORRELATED observations
 * x_1, x_2, \ldots x_N
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

#ifndef CPPSIM_MEAN_HH
#define CPPSIM_MEAN_HH

#include <vector>
#include <stdexcept>
#include "statistic.hh"

class mean : public statistic {
public:
    mean( const string& name, double confl );
    virtual ~mean( );
    virtual void update( double v );
    virtual void reset( void );
    virtual void report( void ) const;
    virtual confInt value( void );

    // Accessors specific for this class

    // Returns the mean (average) of values
    double m( void ) const;

    // Returns the variance
    double variance( void ) const;

protected:
    double _sum;        // Sum of observations
    double _sumsq;      // Sum of squared observations
};

#endif
