/****************************************************************************
 *
 * trremoval_R5.hh
 *
 * This file is part of libcppsim.hh
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

#ifndef TRREMOVAL_R5_HH
#define TRREMOVAL_R5_HH

#include "trremoval.hh"
#include "mean.hh"
#include <vector>
#include <string>

class trremoval_R5 : public trremoval {
public:
    trremoval_R5( const string& name, unsigned int k = 25 ) :
        trremoval( name ),
        _k( k ),
        _mean( "_mean::"+name, .9 )
    { };
    virtual ~trremoval_R5( ) { };
    void update( const double& v );
    void reset( void );
    const vector<double>& value( void );
    void report( void ) const;
protected:
    vector<double> _ob;
    unsigned int _k;
    mean _mean;
};

#endif
