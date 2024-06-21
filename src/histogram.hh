/*****************************************************************************
 *
 * histogram.hh
 *
 * Definition of histograms.
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

#ifndef HISTOGRAM_HH
#define HISTOGRAM_HH

#include <string>
#include <vector>
#include "var.hh"

using namespace std;
    
class histogram : public var< double, double > {
public:
    typedef var< double, double > var_impl;
    
    histogram( const string& name, double a, double b, int nbins );
    virtual ~histogram( );
    void update( double v );
    void reset( void );
    void report( void ) const ;
    double value( void ) { return 0.0; };
protected:
    const double _a;       // lower limit
    const double _b;       // upper limit
    const int _nbins;      // number of bins
    const double _width;   // width of a bin
    double _min;           // Minimum of the values
    double _max;           // Maximum of the values
    vector<int> _values;   // vector of bins
    double _sum;           // Sum of the values
    double _sumsq;         // Sum of the squares of the values
};

#endif





