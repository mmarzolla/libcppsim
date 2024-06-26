/****************************************************************************
 *
 * trremovel_MSERm.hh
 *
 * This file is part of libcppsim
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

#ifndef CPPSIM_TRREMOVAL_MSERM_HH
#define CPPSIM_TRREMOVAL_MSERM_HH

#include <vector>
#include <string>
#include "trremoval.hh"

class trremoval_MSERm : public trremoval {
public:
    trremoval_MSERm( const string& name, unsigned int m = 5 ) :
        trremoval( name ),
        _m( m )
    { };
    virtual ~trremoval_MSERm( ) { };
    void update( const double& v );
    void reset( void );
    const vector<double>& value( void );
    void report( void ) const;
protected:
    vector<double>      _ob;
    unsigned int        _m;
};

#endif
