#ifndef __MIDILLO_MTRK_H
#define __MIDILLO_MTRK_H

#include <istream>
#include <ostream>
#include <list>
#include <midillo/chunk.h>
#include <midillo/event.h>

/**
 * @file
 * @brief MTrk -- track chunk container
 */

namespace midillo {
    using std::istream;
    using std::ostream;

    /**
     * MTrk track chunk container class
     */
    class MTrk_t : public chunk_t {
	public:
	    /**
	     * MIDI events contained in the track
	     */
	    events_t events;

	    /**
	     * Load MTrk chunk from the stream
	     * @param s input stream
	     */
	    void load(istream& s);

	    /**
	     * Save MTrk chunk to the stream
	     * @param s output stream
	     */
	    void save(ostream& s) const;

	    /**
	     * Dump textual representation of MTrk chunk to stream
	     * @param s output stream
	     */
	    void dump(ostream& s) const;
    };

    inline ostream& operator<<(ostream& s,const MTrk_t& mtrk) {
	mtrk.dump(s); return s;
    }

}

#endif /* __MIDILLO_MTRK_H */
