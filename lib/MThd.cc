#include <midillo/MThd.h>
#include <midillo/util.h>
#include <midillo/exception.h>

namespace midillo {
    using std::endl;

    void MThd_t::load(istream& s) {
	header.load(s);
	if(header.id_number!=chunk_id_MThd)
	    throw exception_unexpected_input(CODEPOINT,"MThd chunk expected");
	if(header.length!=6)
	    throw exception_invalid_input(CODEPOINT,"MThd chunk is not 6 bytes long");
	load_data(s);
    }

    void MThd_t::load_data(istream& s) {
	fmt = read16(s);
	ntracks = read16(s);
	division = read16(s);
    }

    void MThd_t::save(ostream& s) const {
	header.save(s);
	write16(s,fmt);
	write16(s,ntracks);
	write16(s,division);
    }

    void MThd_t::dump(ostream& s) const {
	std::ios::fmtflags ff = s.flags();
	s.unsetf(std::ios::hex); s.setf(std::ios::dec);
	s << " " << header << endl
	    << "  fmt=" << fmt << ", " << ntracks << " track(s), division=" << division << endl;
	s.flags(ff);
    }

}
