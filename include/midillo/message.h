#ifndef __MIDILLO_MESSAGE_H
#define __MIDILLO_MESSAGE_H

#include <istream>
#include <ostream>
#include <vector>

/**
 * @file
 * @brief MIDI message
 */

namespace midillo {
    using std::istream;
    using std::ostream;
    using std::vector;

    enum {
	// bits
	status_bit          = 0x80,
	status_event_bits   = 0xF0,
	status_channel_bits = 0x0F,
	status_system_bits  = 0xFF,
	// channel voice messages
	status_note_off                = 0x80,
	status_note_on                 = 0x90,
	status_polyphonic_key_pressure = 0xA0,
	status_aftertouch              = status_polyphonic_key_pressure,
	status_control_change          = 0xB0,
	status_program_change          = 0xC0,
	status_channel_pressure        = 0xD0,
	status_pitch_wheel_change      = 0xE0,
	status_system                  = 0xF0,
	// system common messages
	status_system_sysex                 = 0xF0,
	status_system_MTC_quarter_frame     = 0xF1,
	status_system_song_position_pointer = 0xF2,
	status_system_song_select           = 0xF3,
	status_system_tune_request          = 0xF6,
	status_system_end_of_sysex          = 0xF7,
	// system real-time messages
	status_system_timing_clock          = 0xF8,
	status_system_midi_clock	    = status_system_timing_clock,
	status_system_midi_tick		    = 0xF9,
	status_system_start                 = 0xFA,
	status_system_midi_start	    = status_system_start,
	status_system_continue              = 0xFB,
	status_system_midi_continue	    = status_system_continue,
	status_system_stop                  = 0xFC,
	status_system_midi_stop		    = status_system_stop,
	status_system_active_sense          = 0xFE,
	status_system_rest                  = 0xFF,
	status_system_reset                 = status_system_rest,
	// midi file specific
	status_system_meta                  = 0xFF
    };
    enum {
	// meta events
	meta_sequence_number = 0x00,
	meta_text            = 0x01,
	meta_copyright       = 0x02,
	meta_seq_track_name  = 0x03,
	meta_instrument      = 0x04,
	meta_lyric           = 0x05,
	meta_marker          = 0x06,
	meta_cue_point       = 0x07,
	meta_patch_name      = 0x08,
	meta_port_name       = 0x09,
	meta_EOT             = 0x2F,
	meta_tempo           = 0x51,
	meta_SMPTE_offset    = 0x54,
	meta_time_sig        = 0x58,
	meta_key_sig         = 0x59,
	meta_proprietary     = 0x7F,
	// obsolete meta events
	meta_midi_channel    = 0x20,
	meta_midi_port       = 0x21
    };

    /**
     * MIDI message container
     */
    class message_t {
	public:
	    /**
	     * MIDI status byte
	     */
	    int status;
	    /**
	     * MIDI meta event type
	     */
	    int meta_status;
	    typedef unsigned char byte_t;
	    typedef vector<byte_t> bytes_t;
	    /**
	     * MIDI message data -- content is message-specific
	     */
	    bytes_t data;

	    message_t()
		: status(-1) { }
	    message_t(const message_t& m)
		: status(m.status), meta_status(m.meta_status), data(m.data) { }

	    message_t& operator=(const message_t& m) {
		status = m.status;
		meta_status = m.meta_status;
		data = m.data;
		return *this;
	    }

	    /**
	     * Load MIDI message from the stream
	     * @param rs reference to the running status
	     * @param s input stream
	     */
	    void load(int& rs,istream& s);
	    /**
	     * Save MIDI message to the stream
	     * @param rs reference to the running status
	     * @param s output stream
	     */
	    void save(int& rs,ostream& s) const;
	    /**
	     * Calculate the amount of data that would be written to stream in
	     * case of save()
	     * @param rs reference to the running status
	     * @return the number of bytes
	     */
	    unsigned long calculate_save_size(int& rs) const;

	    /**
	     * Load data so that we have c bytes of data for the event
	     * @param s input stream
	     * @param c size of data needed for the event
	     */
	    void load_data(istream& s,int c);
	    /**
	     * Load sysex data from the stream
	     * @param s input stream
	     */
	    void load_sysex(istream& s);

	    /**
	     * Save data to the stream
	     * @param s output stream
	     */
	    void save_data(ostream& s) const;
	    /**
	     * Save data to the stream and verify if the amount of data is
	     * correct
	     * @param s output stream
	     * @param c data bytes count
	     */
	    void save_data(ostream& s,int c) const;
	    /**
	     * Save sysex data to the stream
	     * @param s output stream
	     */
	    void save_sysex(ostream& s) const;

	    /**
	     * See if the event is meta event
	     * @return true if yes
	     */
	    bool is_meta() const {
		return status==status_system_meta;
	    }
	    /**
	     * Check whether the event is a specific meta event
	     * @param meta meta event type
	     * @return true if yes
	     */
	    bool is_meta(int meta) const {
		return is_meta() && (meta_status==meta);
	    }

	    /**
	     * See if the event is system event
	     * @return true if yes
	     */
	    bool is_system() const {
		return (status&status_event_bits)==status_system && !is_meta();
	    }

	    /**
	     * Dump textual representation of midi message to stream
	     * @param s output stream
	     */
	    void dump(ostream& s) const;

    };

    inline ostream& operator<<(ostream& s,const message_t& m) {
	m.dump(s); return s;
    }

}

#endif /* __MIDILLO_MESSAGE_H */
