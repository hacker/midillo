#include <midillo/event.h>
#include <midillo/util.h>

namespace midillo {

    void event_t::load(int& rs,istream& s) {
	deltat = readVL(s);
	message.load(rs,s);
    }

    void event_t::save(int& rs,ostream& s) const {
	writeVL(s,deltat);
	message.save(rs,s);
    }

    unsigned long event_t::calculate_save_size(int& rs) const {
	return calcVLsize(deltat) + message.calculate_save_size(rs);
    }

    void event_t::dump(ostream& s) const {
	std::ios::fmtflags ff = s.flags();
	s.unsetf(std::ios::hex); s.setf(std::ios::dec);
	s << "deltat=" << deltat << " [" << message << "]";
	s.flags(ff);
    }


    events_t::iterator events_t::append_event() {
	static event_t empty;
	return insert(end(),empty);
    }

    void events_t::load(istream& s) {
	int rs = -1;
	for(;;) {
	    iterator i=append_event();
	    i->load(rs,s);
	    if(i->message.is_meta(meta_EOT))
		break;
	}
    }

    void events_t::save(ostream& s) const {
	int rs = -1;
	for(const_iterator i=begin();i!=end();++i) {
	    i->save(rs,s);
	}
    }

    unsigned long events_t::calculate_save_size() const {
	unsigned long rv = 0;
	int rs = -1;
	for(const_iterator i=begin();i!=end();++i) {
	    rv += i->calculate_save_size(rs);
	}
	return rv;
    }
}
