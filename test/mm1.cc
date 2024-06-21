/****************************************************************************
 *
 * mm1
 *
 * This file is part of cppsim
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
#include <list>
#include <string>
#include <iostream>
#include "cppsim.hh"

extern "C" {
#include <getopt.h>
}

//
// Forward Declarations
//
class job;
class server;
class sink;
class source;

//
// Simulation parameters
//
int numRep = 1; // Number of replications, >=1
                // If > 1, the output will be a set of graphs showing
                // the moving average of the key quantities.
                // If = 1, the output will be a set of confidence
                // intervals for the key quantities.
double confp = 0.9;          // Confidence probability
double simLen = 1e6;         // Sim. Length in time units
bool doDebug = false;
int trLen = 0;               // Length (num of observations) in the transient 
sched_t schedType = SQSDLL;

//
// Entities
//
handle<server>  srv;
handle<sink>    snk;
handle<source>  src;

// 
// Counters, histograms and randoms
//
histogram*      hist;
counter*        c;
statistic*      wtime;          // job's waiting time
statistic*      tput;           // Throughput
counter*        completed;      // Num of completed jobs;
rng<double>*    job_inter_time; // Mean job interarrival time
rng<double>*    job_serv_time;  // Mean job service time

//
// class SERVER
//
class server: public process {
public:
    server( const string& name ) : 
	process( name ) { };
    list< handle<job> > queue;
protected:
    void inner_body( void );
};


//
// class SINK
//
class sink: public process {
public:
    sink( const string& name ) : 
	process( name ) { };
    list< handle<job> > queue;
protected:
    void inner_body( void );
};


//
// class SOURCE
//
class source: public process {
public:
    source( const string& name ) : 
	process( name ) { };
protected:
    void inner_body( void );
};


//
// class JOB
//
class job: public process {
public:
    job( const string& name ) : 
	process( name ),
	createdAt( time( ) ),
	servicedAt( 0.0 ) { };
    double createdAt;
    double servicedAt;
protected:
    void inner_body( void );
};


class simulation_main: public process {
public:
    simulation_main( const string& name ) : 
	process( name ) { };
protected:
    void inner_body( void );
};


//
// Body Definitions
//
void source::inner_body( void )
{    
    while ( 1 ) {
        hold( job_inter_time->value( ) );
        handle<job> j( new job( "Job" ) );
        j->activateAfter( current( ) );
    }
}

void job::inner_body( void )
{
    if ( srv->idle( ) ) 
        srv->activateAfter( current( ) );
    srv->queue.push_back( this );

    passivate( ); // wait for service

    wtime->update( servicedAt - createdAt );
    hist->update( servicedAt - createdAt );

    completed->update( 1.0 );
    tput->update( completed->value( ) / time( ) );

    if ( snk->idle( ) )
	snk->activateAfter( current( ) );
    snk->queue.push_back( this );
}

void sink::inner_body( void )
{
    while ( 1 ) {
        if ( queue.empty( ) )
            passivate( );
        queue.pop_front( );
    }    
}

void server::inner_body( void )
{
    double t_stop    = 0.0;
    double t_restart = 0.0;

    while ( 1 ) {
        if ( queue.empty( ) ) {
	    t_stop = time( );
	    c->update( t_restart - t_stop );
            passivate( );
	    t_restart = time( );
	}
        handle<job> j( queue.front( ) );
        queue.pop_front( );
        j->servicedAt = time( );
        hold( job_serv_time->value( ) );
	j->activateAfter( current( ) );
    }    
}

void simulation_main::inner_body( void )
{
    srv = new server( "Server" );
    snk = new sink( "Sink" );
    src = new source( "Source" );
    
    src->activate( );
    srv->activate( );
    snk->activate( );
    
    hold( simLen );
    
    // Cleanup processes
    src->cancel( );
    srv->cancel( );
    snk->cancel( );

    end_simulation( );
}

int main( int argc, char* argv[] )
{
    char opt;

    /*
     * Read the command line parameters
     */
    while ( -1 != ( opt = getopt( argc, argv, "c:hr:l:t:DS:" ) ) ) {
	switch (opt) {
	case 'c':
	    confp = atof( optarg );
	    break;
	case 'h':
	    cout << "Usage: " << argv[0] 
		 << " [-c <confp>] [-h] [-r <numrep>] [-l <siml>] [-t <trlen>] [-D] [-S]" << endl
		 << endl
		 << " -c <confp>  Sets the confidence probability to <confp> (def: 0.9)" << endl
		 << " -r <numrep> Sets the number of replications to <numrep> (def: 1)" << endl
		 << " -l <siml>   Sets the length of the simulation, in time units (def 1e6)" << endl
		 << " -t <trlen>  Length of the transient period (def 0)" << endl
		 << " -D          Turns on lots of debugging messages (def off)" << endl
		 << " -S          Sqs type (0 = Doubly linked List, 1 = Priority Queue)" << endl
		 << " -h          Displays this help" << endl
		 << endl;
	    exit( 0 );
	    break;
	case 'r':
	    numRep = atoi( optarg );
	    assert( numRep >= 1 );
	    break;
	case 'l':
	    simLen = atof( optarg );
	    break;
	case 't':
	    trLen = atoi( optarg );
	    break;
	case 'D':
	    doDebug = true;
	    process::getLogger()->setLevel( 0 );
	    break;
	case 'S':
	    schedType = (sched_t)atoi( optarg );
	    break;
	default:
	    cerr << "Unrecognized option: " << argv[ optind ] << endl;
	    exit( -1 );
	}
	
    }

    //
    // Initialize the variables
    //
    if ( numRep > 1 ) {
        wtime           = new welch( "Waiting_Time", .95, 500 );
        tput            = new welch( "Throughput", .95, 500 );
    } else {
        wtime           = new bmeans2( "Waiting_Time", confp );
        tput            = new bmeans2( "Throughput", confp );
    }
    completed           = new counter( "Completed" );
    job_inter_time      = new rngExp( "Job Interarr. Time", 1.0/0.09 );
    job_serv_time       = new rngExp( "Service Time", 1.0/0.1 );
    hist                = new histogram("Waiting Time", 50.0, 100.0, 100 );
    c                   = new counter("Busy Time");
    
    cout << "Replication ";
    for ( int i=0; i<numRep; i++ ) {        

	    if ( i>0 ) {	    // Prepare variables for the run
        	wtime->reset( );
        	completed->reset( );
        	tput->reset( );
        	hist->reset( );
        	c->reset( );
        	job_inter_time->reset( );
        	job_serv_time->reset( );        
	    }

        if ( 0 == i % 10 )
            cout << i << flush;
        else
            cout << "." << flush;

        simulation::instance()->begin_simulation( makeSqs( schedType ) );
        // simulation::instance()->SQS()->getLogger()->setOutputFile( new ofstream( "sqs.dat" ) );
        // simulation::instance()->SQS()->getLogger()->setLevel( 0 );
        handle<simulation_main> 
            sim_main1( new simulation_main( "Simulation main1" ) );
        sim_main1->activate( );
        simulation::instance()->run();
        simulation::instance()->end_simulation();
    }
    cout << "done!" << endl;
    try {
        wtime->report( );
        tput->report( );
    } catch ( runtime_error& e ) {
        cerr << "Runtime Error Caught: " << e.what() << endl;
    }

    return 0;
}
