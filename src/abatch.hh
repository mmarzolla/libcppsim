/*****************************************************************************
 *
 * abatch.hh
 *
 * Implementation of the ABATCH strategy
 *
 * Copyright (C) 2002,2003 Moreno Marzolla
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

#ifndef _CPPSIM_ABATCH_HH
#define _CPPSIM_ABATCH_HH

#include <vector>
#include "statistic.hh"
#include <fstream>

using namespace std;

class abatch : public statistic {
public:
    abatch( const string& name, 
	    double confp,               // The confidence level (1-\alpha)
            unsigned int sampsize,      // Number of observations
	    unsigned int transientLen=0 // The length of the transient 
	    );
    virtual ~abatch( );
    virtual void update( const double& v );
    virtual void reset( void );       
    virtual void report( void ) const;
    virtual confInt value( void );

protected:

    void process_batch( void );

    // True iff H0 is accepted
    bool H0_is_accepted( const vector<double> &Y, const double xbar, double& p );

    const double alpha;         
    const unsigned int b_1;
    const unsigned int k_1;
    const unsigned int n;
    unsigned int b;             // Current batch size
    unsigned int k;             // Number of batches of size b
    unsigned int N;             // Number of observations
    double btilde_1;
    unsigned int ktilde_1;
    unsigned int ntilde;
    unsigned int transientLen;  // Transient length
    unsigned int L;             // Number of "periods"
    double f, g;
    vector<double> ob;          // Vectors of observations
    double sum;                 // Sum of the observations
    ofstream os;
};

#endif
