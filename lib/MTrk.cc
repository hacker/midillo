#include <algorithm>
#include <iterator>
#include <midillo/MTrk.h>
#include <midillo/exception.h>

namespace midillo {
    using std::copy;
    using std::ostream_iterator;
    using std::endl;

    void MTrk_t::load(istream& s) {
	header.load(s);
	if(header.id_number!=chunk_id_MTrk)
	    throw exception_unexpected_input(CODEPOINT,"MTrk chunk expected");
	events.load(s);
    }

    void MTrk_t::save(ostream& s) const {
	chunk_header_t h = header;
	h.id_number = chunk_id_MTrk;
	h.length = events.calculate_save_size();
	h.save(s);
	events.save(s);
    }

    void MTrk_t::dump(ostream& s) const {
	s << " " << header << endl
	    << "  ";
	copy(
		events.begin(), events.end(),
		ostream_iterator<event_t>(s,"\n  ") );
    }

}
