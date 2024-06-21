/****************************************************************************
 *
 * quantile.hh
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

#ifndef QUANTILE_HH
#define QUANTILE_HH

#include <vector>
#include "statistic.hh"

//
// Computes the p-quantile of the observations
//
class quantile : public statistic {
public:
    quantile( const string& name, double p, double confl );
    virtual ~quantile( );

    void update( double v );
    void reset( void );
    confInt value( void );
    void report( void ) const;
protected:
    const double        _p;
    vector< double >    _ob;
};

#endif
