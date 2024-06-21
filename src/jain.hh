/****************************************************************************
 *
 * jain.hh
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

#ifndef CPPSIM_JAIN_HH
#define CPPSIM_JAIN_HH

#include <vector>
#include <string>
#include "statistic.hh"

class jain : public statistic {
public:
    jain( const string& name,
          double confl );
    virtual ~jain( );
    void update( double v );
    void reset( void );
    void report( void ) const;
protected:
    confInt value( void );

    vector< vector<double> > _ob;
};

#endif
