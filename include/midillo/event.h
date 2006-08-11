#ifndef __MIDILLO_EVENT_H
#define __MIDILLO_EVENT_H

#include <istream>
#include <ostream>
#include <list>
#include <midillo/message.h>

/**
 * @file
 * @brief midi event container
 */

namespace midillo {
    using std::istream;
    using std::ostream;
    using std::list;

    /**
     * MIDI event container class
     */
    class event_t {
	public:
	    /**
	     * delta time since the last event
	     */
	    long deltat;
	    /**
	     * MIDI message itself
	     */
	    message_t message;

	    /**
	     * Load MIDI event from the stream
	     * @param rs reference to the running status
	     * @param s input stream
	     */
	    void load(int& rs,istream& s);

	    /**
	     * Save MIDI event to the stream
	     * @param rs reference to the running status
	     * @param s output stream
	     */
	    void save(int& rs,ostream& s) const;

	    /**
	     * Calculate the amount of data that would be written to stream in
	     * case of save
	     * @param rs reference to the running status
	     * @return the number of bytes
	     */
	    unsigned long calculate_save_size(int& rs) const;

	    /**
	     * Dump textual representation of event to stream
	     * @param s output stream
	     */
	    void dump(ostream& s) const;
    };

    inline ostream& operator<<(ostream& s,const event_t& e) {
	e.dump(s); return s;
    }

    /**
     * MIDI events list container
     */
    class events_t : public list<event_t> {
	public:

	    /**
	     * Append empty event to the end of the list
	     * @return iterator, pointing to the appended event
	     */
	    iterator append_event();

	    /**
	     * Load MIDI events (track data) from the stream
	     * @param s input stream
	     */
	    void load(istream& s);
	    /**
	     * Save MIDI events (track data) to the stream
	     * @param s output stream
	     */
	    void save(ostream& s) const;

	    /**
	     * Calculate the size of the track data that would be written to
	     * the stream by save()
	     * @return the number of bytes
	     */
	    unsigned long calculate_save_size() const;
    };

}

#endif /* __MIDILLO_EVENT_H */
