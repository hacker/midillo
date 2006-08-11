#ifndef __MIDILLO_SMF_H
#define __MIDILLO_SMF_H

#include <istream>
#include <ostream>
#include <list>
#include <midillo/MThd.h>
#include <midillo/MTrk.h>

/**
 * @file
 * @brief the SMF_t -- standard midi file
 */

namespace midillo {
    using std::istream;
    using std::vector;
    using std::ostream;

    /**
     * Standard midi file object
     */
    class SMF_t {
	public:
	    /**
	     * MThd header chunk
	     */
	    MThd_t mthd;
	    /**
	     * The type for collection of MTrk track chunks
	     */
	    typedef list<MTrk_t> tracks_t;
	    /**
	     * MTrk track chunks collection for the file
	     */
	    tracks_t tracks;

	    SMF_t() { }
	    /**
	     * Construct object from the file
	     * @see load(const char *f,bool stdinable)
	     */
	    SMF_t(const char *f,bool stdinable=true) { load(f,stdinable); }
	    /**
	     * Construct object from the stream
	     * @see load(istream& s)
	     */
	    SMF_t(istream& s) { load(s); }

	    /**
	     * Load MIDI data from the file
	     * @param f filename
	     * @param stdinable true if '-' is treatead as cin input stream
	     */
	    void load(const char *f,bool stdinable=true);
	    /**
	     * Load midi data from the stream
	     * @param s input stream
	     */
	    void load(istream& s);

	    /**
	     * Save MIDI data to the file
	     * @param f filename
	     * @param stdoutable true if '-' is treated as cout output stream
	     */
	    void save(const char *f,bool stdoutable=true) const;
	    /**
	     * Save MIDI data to the stream
	     * @param s output stream
	     */
	    void save(ostream& s) const;

	    /**
	     * Dump textual representation of SMF to stream
	     * @param s output stream
	     */
	    void dump(ostream& s) const;
    };

    inline ostream& operator<<(ostream& s,const SMF_t& smf) {
	smf.dump(s); return s;
    }

}

#endif /* __MIDILLO_SMF_H */
