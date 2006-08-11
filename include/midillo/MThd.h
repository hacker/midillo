#ifndef __MIDILLO_MTHD_H
#define __MIDILLO_MTHD_H

#include <istream>
#include <ostream>
#include <midillo/chunk.h>

/**
 * @file
 * @brief the MThd_t -- MThd header chunk class
 */

namespace midillo {
    using std::istream;
    using std::ostream;

    /**
     * MThd header chunk container
     */
    class MThd_t : public chunk_t {
	public:
	    enum {
		fmt_0 = 0, fmt_1 = 1, fmt_2 = 2,
		fmt_singletrack = fmt_0,
		fmt_multitrack = fmt_1,
		fmt_tracksequence = fmt_2
	    };
	    /**
	     * SMF format. 0 for single track, 1 for multitrack, 2 for track
	     * sequence.
	     */
	    int fmt;
	    /**
	     * Number of tracks in the file
	     */
	    int ntracks;
	    /**
	     * The number of pulses per quarter note
	     */
	    int division;

	    /**
	     * Load MThd chunk from the stream
	     * @param s input stream
	     */
	    void load(istream& s);

	    /**
	     * Read MThd chunk data from the stream. This function assumes that
	     * header is already read.
	     * @param s input stream
	     */
	    void load_data(istream& s);

	    /**
	     * Save MThd chunk to the stream
	     * @param s output stream
	     */
	    void save(ostream& s) const;

	    /**
	     * Dump textual representation of MThd chunk to stream
	     * @param s output stream
	     */
	    void dump(ostream& s) const;
    };

    inline ostream& operator<<(ostream& s,const MThd_t& mthd) {
	mthd.dump(s); return s;
    }

}

#endif /* MIDILLO_MTHD_H */
