#include <getopt.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;
#include <konforka/exception.h>
#include <midillo/SMF.h>
using namespace midillo;

#include "config.h"
#define PHEADER PACKAGE " " VERSION " - midifilter - midi filter tool"
#define PCOPY "Copyright (c) 2006 Klever Group"

enum {
# include "filters_enumeration.cc"
    total_filters
};

struct filter_t {
    const char *ids[5];
    const char *help;
    bool filter;
} filters[] = {
# include "filters_definition.cc"
  {0,0,0}
};
#define FILTER(f) filters[filter_##f].filter

inline ostream& operator<<(ostream& s,const filter_t& f) {
    ios::fmtflags ff = s.flags();
    int w = s.width(25);
    s.unsetf(ios::right); s.setf(ios::left);
    s << *f.ids << " " << f.help;
    s.width(w);
    s.flags(ff);
    return s;
}

static bool message_is_filtered(const message_t& m) {
    if(m.is_meta(meta_EOT))
	return false;

    if(FILTER(sysex) && (m.status==status_system_sysex || m.status==status_system_end_of_sysex))
	return true;
    if(FILTER(MTC_quarter_frame) && m.status==status_system_MTC_quarter_frame)
	return true;
    if(FILTER(song_position_pointer) && m.status==status_system_song_position_pointer)
	return true;
    if(FILTER(song_select) && m.status==status_system_song_select)
	return true;
    if(FILTER(tune_request) && m.status==status_system_tune_request)
	return true;
    if(FILTER(midi_clock) && m.status==status_system_midi_clock)
	return true;
    if(FILTER(midi_tick) && m.status==status_system_midi_tick)
	return true;
    if(FILTER(midi_start) && m.status==status_system_midi_start)
	return true;
    if(FILTER(midi_continue) && m.status==status_system_midi_continue)
	return true;
    if(FILTER(midi_stop) && m.status==status_system_midi_stop)
	return true;
    if(FILTER(active_sense) && m.status==status_system_active_sense)
	return true;

    if(FILTER(meta_sequence_number) && m.is_meta(meta_sequence_number))
	return true;
    if(FILTER(meta_text) && m.is_meta(meta_text))
	return true;
    if(FILTER(meta_copyright) && m.is_meta(meta_copyright))
	return true;
    if(FILTER(meta_seq_track_name) && m.is_meta(meta_seq_track_name))
	return true;
    if(FILTER(meta_instrument) && m.is_meta(meta_instrument))
	return true;
    if(FILTER(meta_lyric) && m.is_meta(meta_lyric))
	return true;
    if(FILTER(meta_marker) && m.is_meta(meta_marker))
	return true;
    if(FILTER(meta_cue_point) && m.is_meta(meta_cue_point))
	return true;
    if(FILTER(meta_patch_name) && m.is_meta(meta_patch_name))
	return true;
    if(FILTER(meta_port_name) && m.is_meta(meta_port_name))
	return true;
    if(FILTER(meta_tempo) && m.is_meta(meta_tempo))
	return true;
    if(FILTER(meta_SMPTE_offset) && m.is_meta(meta_SMPTE_offset))
	return true;
    if(FILTER(meta_time_sig) && m.is_meta(meta_time_sig))
	return true;
    if(FILTER(meta_key_sig) && m.is_meta(meta_key_sig))
	return true;
    if(FILTER(meta_proprietary) && m.is_meta(meta_proprietary))
	return true;

    if(FILTER(meta_midi_channel) && m.is_meta(meta_midi_channel))
	return true;
    if(FILTER(meta_midi_port) && m.is_meta(meta_midi_port))
	return true;

    if(FILTER(meta_unknown) && m.is_meta()) {
	const int known_metas[] = {
	    meta_sequence_number, meta_text, meta_copyright, meta_seq_track_name,
	    meta_instrument, meta_lyric, meta_marker, meta_cue_point,
	    meta_patch_name, meta_port_name, meta_EOT, meta_tempo,
	    meta_SMPTE_offset, meta_time_sig, meta_key_sig, meta_proprietary,
	    meta_midi_channel, meta_midi_port };
	const int* lastknown = &known_metas[sizeof(known_metas)/sizeof(*known_metas)];
	if( find(
		    known_metas, lastknown,
		    m.meta_status ) == lastknown ) {
	    return true;
	}
    }

    if(FILTER(meta) && m.is_meta())
	return true;

    return false;
}

static bool event_is_filtered(const event_t& e) {
    return message_is_filtered(e.message);
}

static bool MTrk_is_empty(const MTrk_t& t) {
    return 
	t.events.empty()
	|| t.events.front().message.is_meta(meta_EOT);
}

struct filter_preset_t {
    const char *id;
    const char *help;
    int filters[total_filters];
} filter_presets[] = {
    { "system_common", "strip system common messages",
	{ filter_sysex, filter_song_position_pointer, filter_song_select,
	    filter_tune_request, -1 } },
    { "system_runtime", "strip system runtime messages",
	{ filter_midi_clock, filter_midi_tick, filter_midi_start,
	    filter_midi_continue, filter_midi_stop, filter_active_sense, -1 } },
    { "meta_obsolete", "strip obsolete meta events",
	{ filter_meta_midi_channel, filter_meta_midi_port, -1 } },
    { "meta_texts", "strip textual meta events",
	{ filter_meta_text, filter_meta_copyright,
	    filter_meta_seq_track_name, filter_meta_instrument,
	    filter_meta_lyric, filter_meta_marker, filter_meta_cue_point,
	    filter_meta_patch_name, filter_meta_port_name, -1 } }
};

inline ostream& operator<<(ostream& s,const filter_preset_t& fp) {
    ios::fmtflags ff = s.flags();
    int w = s.width(25);
    s.unsetf(ios::right); s.setf(ios::left);
    s << fp.id << " " << fp.help;
    s.width(w);
    s.flags(ff);
    return s;
}

static void usage(const char *p) {
    cerr << PHEADER << endl
	<< PCOPY << endl << endl
	<< " " << p << " [options] [<input-file>[ <output-file>]]" << endl << endl
	<< " -h, --help" << endl
	<< " --usage          display this text" << endl
	<< " -V, --version    display version number" << endl
	<< " -L, --license    show license" << endl
	<< " -f <filters>, --filter=<filters>"
	<< "                  specify the list of events (comma-separated) to" << endl
	<< "                  strip" << endl
	<< " -l, --list-filters" << endl
	<< "                  list available filters" << endl;
}

main(int argc,char **argv) {
    try {
	while(true) {
	    static struct option opts[] = {
		{ "help", no_argument, 0, 'h' },
		{ "usage", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "license", no_argument, 0, 'L' },
		{ "filter", no_argument, 0, 'f' },
		{ "list-filters", no_argument, 0, 'l' },
		{ NULL, 0, 0, 0 }
	    };
	    int c = getopt_long(argc,argv,"f:hVLl",opts,NULL);
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
		case 'f':
		    {
			string fs = optarg;
			while(!fs.empty()) {
			    string::size_type ns = fs.find_first_not_of(" :/,;");
			    if(ns==string::npos)
				break;
			    if(ns)
				fs.erase(ns);
			    string::size_type s = fs.find_first_of(" :/,;");
			    string f;
			    if(s==string::npos) {
				f = fs; fs.clear();
			    }else{
				f = fs.substr(0,ns);
				fs.erase(0,ns+1);
			    }
			    for(int fn=0;fn<total_filters;++fn) {
				filter_t& filter = filters[fn];
				for(int fid=0;fid<(sizeof(filter.ids)/sizeof(*filter.ids)) && filter.ids[fid];++fid) {
				    if(f == filter.ids[fid])
					filter.filter = true;
				}
			    }
			}
		    }
		    break;
		case 'l':
		    cerr << PHEADER << endl
			<< PCOPY << endl << endl;
		    copy(
			    filters, &filters[total_filters],
			    ostream_iterator<filter_t>(cerr,"\n") );
		    copy(
			    filter_presets, &filter_presets[sizeof(filter_presets)/sizeof(*filter_presets)],
			    ostream_iterator<filter_preset_t>(cerr,"\n") );
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
	for(SMF_t::tracks_t::iterator t=in.tracks.begin();t!=in.tracks.end();++t) {
	    for(events_t::iterator e=t->events.begin();e!=t->events.end();) {
		if(event_is_filtered(*e)) {
		    events_t::iterator i = e++;
		    // we assume it is not the last event, since the last event
		    // (meta_EOT) is unfilterable
		    e->deltat += i->deltat;
		    t->events.erase(i);
		}else
		    ++e;
	    }
	}
	in.tracks.erase(
		remove_if(
		    in.tracks.begin(),in.tracks.end(),
		    MTrk_is_empty),
		in.tracks.end() );
	if(in.tracks.empty()) {
	    cerr << "We have no MIDI data in the output file";
	}
	in.mthd.ntracks = in.tracks.size();
	in.save(oufile);
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
