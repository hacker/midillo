#include <getopt.h>
#include <iostream>
#include <algorithm>
using namespace std;
#include <konforka/exception.h>
#include <midillo/SMF.h>
using namespace midillo;

#include "config.h"
#define PHEADER PACKAGE " " VERSION " - midi2f0 - convert to midi format 0"
#define PCOPY "Copyright (c) 2006 Klever Group"

static void usage(const char *p) {
    cerr << PHEADER << endl
	<< PCOPY << endl << endl
	<< " " << p << " [options] [<input-file>[ <output-file>]]" << endl << endl
	<< " -h, --help" << endl
	<< " --usage          display this text" << endl
	<< " -V, --version    display version number" << endl
	<< " -L, --license    show license" << endl;
}

static bool MTrk_is_empty(const MTrk_t& t) {
    return 
	t.events.empty()
	|| t.events.front().message.is_meta(meta_EOT);
}

static bool MTrk_deltat_lt(const MTrk_t& a,const MTrk_t& b) {
    // we assume tracks aren't empty
    return a.events.front().deltat < b.events.front().deltat;
}

static bool event_has_nonzero_deltat_or_is_EOT(const event_t& e) {
    return e.deltat!=0 || e.message.is_meta(meta_EOT);
}

main(int argc,char **argv) {
    try {
	while(true) {
	    static struct option opts[] = {
		{ "help", no_argument, 0, 'h' },
		{ "usage", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "license", no_argument, 0, 'L' },
		{ NULL, 0, 0, 0 }
	    };
	    int c = getopt_long(argc,argv,"hVL",opts,NULL);
	    if(c==-1)
		break;
	    switch(c) {
		case 'h':
		    usage(*argv);
		    exit(0);
		    break;
		case 'V':
		    cerr << VERSION << endl;
		    exit(0);
		    break;
		case 'L':
		    extern const char *COPYING;
		    cerr << COPYING << endl;
		    exit(0);
		    break;
		default:
		    cerr << "Huh??" << endl;
		    break;
	    }
	}
	const char *infile = "-";
	if(optind<argc)
	    infile = argv[optind++];
	const char *oufile = "-";
	if(optind<argc)
	    oufile = argv[optind++];
	if(optind<argc) {
	    usage(*argv);
	    exit(1);
	}
	SMF_t in(infile);
	if(in.mthd.fmt==MThd_t::fmt_0) {
	    cerr << "File is already SMF 0" << endl;
	    in.save(oufile);
	}else if(in.mthd.fmt==MThd_t::fmt_1) {
	    cerr << "Converting from SMF 1 to SMF 0" << endl;
	    SMF_t ou;
	    ou.mthd = in.mthd;
	    ou.mthd.fmt = MThd_t::fmt_0; ou.mthd.ntracks = 1;
	    ou.tracks.resize(1);
	    MTrk_t& mtrk = ou.tracks.front();
	    events_t& evs = mtrk.events;
	    for(;;) {
		// Cleaning up empty tracks
		in.tracks.erase(
			remove_if(
			    in.tracks.begin(),in.tracks.end(),
			    MTrk_is_empty),
			in.tracks.end() );
		if(in.tracks.empty())
		    break;
		// Find the least deltat
		unsigned long mindt = min_element(
			in.tracks.begin(), in.tracks.end(),
			MTrk_deltat_lt )->events.front().deltat;
		int t=0;
		bool reset = false;
		for(SMF_t::tracks_t::iterator i=in.tracks.begin();i!=in.tracks.end();++i) {
		    if(i->events.front().deltat > mindt) {
			i->events.front().deltat-=mindt;
		    }else{
			do{
			    evs.splice(
				    evs.end(),
				    i->events,i->events.begin() );
			    if(reset)
				evs.back().deltat = 0;
			    else 
				reset = true;
			    events_t::iterator lze=find_if(
				    i->events.begin(),i->events.end(),
				    event_has_nonzero_deltat_or_is_EOT );
			    evs.splice(
				    evs.end(),
				    i->events,i->events.begin(),lze );
			}while( (!MTrk_is_empty(*i)) && i->events.front().deltat==0 );
		    }
		}
	    }
	    event_t& eot = *evs.append_event();
	    eot.deltat=0;
	    eot.message.status = status_system_meta;
	    eot.message.meta_status = meta_EOT;
	    ou.save(oufile);
	}else if(in.mthd.fmt==MThd_t::fmt_2) {
	    // TODO
	    cerr << "Not implemented" << endl;
	}else{
	    cerr << "Unknown MIDI file format" << endl;
	}
	return 0;
    }catch(konforka::exception& e) {
	cerr << "Oops... Konforka exception:" << endl
	    << " what: " << e.what() << endl
	    << " where: " << e.where() << endl;
	return 1;
    }catch(exception& e) {
	cerr << "Oops... Exception:" << endl
	    << " what: " << e.what() << endl;
	return 1;
    }
}
