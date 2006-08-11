#include <algorithm>
#include <iterator>
#include <midillo/message.h>
#include <midillo/util.h>
#include <midillo/exception.h>

namespace midillo {
    using std::copy;
    using std::ostream_iterator;

    unsigned long message_t::calculate_save_size(int& rs) const {
	unsigned long rv = 0;
	if(status!=rs) {
	    ++rv;
	    rs = status;
	}else if((status&status_event_bits)==status_system) {
	    rs = -1;
	    ++rv; // XXX: is it really needed?
	}
	switch(status&status_event_bits) {
	    case status_note_off:
	    case status_note_on:
	    case status_polyphonic_key_pressure: // aka status_aftertouch
	    case status_control_change:
	    case status_pitch_wheel_change:
		rv += 2; break;
	    case status_program_change:
	    case status_channel_pressure:
		++rv; break;
	    case status_system:
		switch(status&status_system_bits) {
		    case status_system_sysex:
		    case status_system_end_of_sysex:
			rv += data.size()+1; break;
		    case status_system_MTC_quarter_frame:
		    case status_system_song_select:
			++rv; break;
		    case status_system_song_position_pointer:
			rv += 2; break;
		    case status_system_tune_request:
		    case status_system_timing_clock: // aka status_system_midi_clock
		    case status_system_midi_tick:
		    case status_system_start: // aka status_system_midi_start
		    case status_system_stop: // aka status_system_midi_stop
		    case status_system_continue: // aka status_system_midi_continue
		    case status_system_active_sense:
			break; /* XXX: ensure there is no data? */
		    case status_system_meta: // also reset, but not for the purpose of midi file
			++rv;
			rv += calcVLsize(data.size());
			rv += data.size();
			break;
		    default:
			throw exception(CODEPOINT,"Internal error");
			break;
		}
		break;
	    default:
		throw exception(CODEPOINT,"Internal error");
		break;
	}
	return rv;
    }

    void message_t::save(int& rs,ostream& s) const {
	if(status!=rs) {
	    s.put(status);
	    if(!s.good())
		throw exception_output_error(CODEPOINT,"Error writing midi status byte");
	    rs = status;
	}else if((status&status_event_bits)==status_system) {
	    rs = -1;
	    s.put(status); // XXX: is it really needed?
	    if(!s.good())
		throw exception_output_error(CODEPOINT,"Error writing midi system status byte");
	}
	switch(status&status_event_bits) {
	    case status_note_off:
	    case status_note_on:
	    case status_polyphonic_key_pressure: // aka status_aftertouch
	    case status_control_change:
	    case status_pitch_wheel_change:
		save_data(s,2); break;
	    case status_program_change:
	    case status_channel_pressure:
		save_data(s,1); break;
	    case status_system:
		switch(status&status_system_bits) {
		    case status_system_sysex:
		    case status_system_end_of_sysex:
			save_sysex(s); break;
		    case status_system_MTC_quarter_frame:
		    case status_system_song_select:
			save_data(s,1); break;
		    case status_system_song_position_pointer:
			save_data(s,2); break;
		    case status_system_tune_request:
		    case status_system_timing_clock: // aka status_system_midi_clock
		    case status_system_midi_tick:
		    case status_system_start: // aka status_system_midi_start
		    case status_system_stop: // aka status_system_midi_stop
		    case status_system_continue: // aka status_system_midi_continue
		    case status_system_active_sense:
			break; /* XXX: ensure there is no data? */
		    case status_system_meta: // also reset, but not for the purpose of midi file
			s.put(meta_status&0xFF);
			if(!s.good())
			    throw exception_output_error(CODEPOINT,"Error writing meta event");
			writeVL(s,data.size());
			save_data(s);
			break;
		    default:
			throw exception(CODEPOINT,"Internal error");
			break;
		}
		break;
	    default:
		throw exception(CODEPOINT,"Internal error");
		break;
	}
    }

    void message_t::load(int& rs,istream& s) {
	data.clear();
	status = s.get();
	if(!s.good())
	    throw exception_input_error(CODEPOINT,"Error reading MIDI event status byte");
	if(status&status_bit) {
	    if((status&status_event_bits)!=status_system)
		rs = status;
	}else{
	    if(rs<0)
		throw exception_invalid_input(CODEPOINT,"Attempt to rely on the absent running status");
	    data.push_back(status);
	    status = rs;
	}
	switch(status&status_event_bits) {
	    case status_note_off:
	    case status_note_on:
	    case status_polyphonic_key_pressure: // a.k.a. status_aftertouch
	    case status_control_change:
	    case status_pitch_wheel_change:
		load_data(s,2); break;
	    case status_program_change:
	    case status_channel_pressure:
		load_data(s,1); break;
	    case status_system:
		switch(status&status_system_bits) {
		    case status_system_sysex:
		    case status_system_end_of_sysex:
			load_sysex(s); break;
		    case status_system_MTC_quarter_frame:
		    case status_system_song_select:
			load_data(s,1); break;
		    case status_system_song_position_pointer:
			load_data(s,2); break;
		    case status_system_tune_request:
		    case status_system_timing_clock: // a.k.a. status_system_midi_clock
		    case status_system_midi_tick:
		    case status_system_start: // a.k.a. status_system_midi_start
		    case status_system_stop: // a.k.a. status_system_midi_stop
		    case status_system_continue: // a.k.a. status_system_midi_continue
		    case status_system_active_sense:
			break;
		    case status_system_meta: // also, reset, but not in midi files
			{
			    meta_status = s.get();
			    if(!s.good())
				throw exception_input_error(CODEPOINT,"Error reading meta event type");
			    int l = readVL(s);
			    load_data(s,l);
			}
			break;
		    default:
			throw exception(CODEPOINT,"Internal error");
			break;
		}
		break;
	    default:
		throw exception(CODEPOINT,"Internal error");
		break;
	}
    }

    void message_t::save_data(ostream& s,int c) const {
	if(c!=data.size())
	    throw exception(CODEPOINT,"Writing corrupt data");
	save_data(s);
    }

    void message_t::save_data(ostream& s) const {
	for(bytes_t::const_iterator i=data.begin();i!=data.end();++i)
	    s.put(*i);
	if(!s.good())
	    throw exception_output_error(CODEPOINT,"Error writing MIDI message data");
    }

    void message_t::load_data(istream& s,int c) {
	c -= data.size();
	if(c<0)
	    throw exception(CODEPOINT,"Internal error");
	while(c-- > 0) {
	    data.push_back(s.get());
	    if(!s.good())
		throw exception_input_error(CODEPOINT,"Error reading MIDI data");
	}
    }

    void message_t::save_sysex(ostream& s) const {
	save_data(s);
	s.put(0xF7); /* XXX: Or is it better to put it into data? */
	if(!s.good())
	    throw exception_output_error(CODEPOINT,"Error writing sysex data");
    }

    void message_t::load_sysex(istream& s) {
	int b = s.get();
	if(!s.good())
	    throw exception_input_error(CODEPOINT,"Error reading sysex data");
	assert(!(b&0x80)); // manufacturer ought to be 7 bit. not sure if it belongs here, it may well be continuation.
	do {
	    data.push_back(b);
	    b = s.get();
	    if(!s.good())
		throw exception_input_error(CODEPOINT,"Error reading sysex data");
	}while(b!=0xF7);
    }

    void message_t::dump(ostream& s) const {
	std::ios::fmtflags ff = s.flags();
	int w = s.width(2);
	s.unsetf(std::ios::dec); s.setf(std::ios::hex);
	s << "status=" << status;
	if(is_meta()) {
	    s << ", meta_status=" << meta_status;
	}
	if(!data.empty()) {
	    s << ", data: ";
	    copy(
		    data.begin(), data.end(),
		    ostream_iterator<int>(s," ") );
	}
	s.width(w);
	s.flags(ff);
    }

}
