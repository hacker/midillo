#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <midillo/SMF.h>

namespace midillo {
    using std::ifstream;
    using std::ofstream;
    using std::cin;
    using std::cout;
    using std::copy;
    using std::ostream_iterator;
    using std::endl;

    void SMF_t::load(const char *f,bool stdinable) {
	if(stdinable && !strcmp(f,"-")) {
	    load(cin);
	}else{
	    ifstream s(f,std::ios::in|std::ios::binary);
	    load(s);
	}
    }

    void SMF_t::load(istream& s) {
	mthd.load(s);
	tracks.resize(mthd.ntracks);
	tracks_t::iterator i = tracks.begin();
	for(int t=0;t<mthd.ntracks;++t,++i) {
	    i->load(s);
	}
    }

    void SMF_t::save(const char *f,bool stdoutable) const {
	if(stdoutable && !strcmp(f,"-")) {
	    save(cout);
	}else{
	    ofstream s(f,std::ios::out|std::ios::trunc|std::ios::binary);
	    save(s);
	}
    }

    void SMF_t::save(ostream& s) const {
	mthd.save(s);
	for(tracks_t::const_iterator i=tracks.begin();i!=tracks.end();++i) {
	    i->save(s);
	}
    }

    void SMF_t::dump(ostream& s) const {
	std::ios::fmtflags ff = s.flags();
	s.unsetf(std::ios::hex); s.setf(std::ios::dec);
	s
	    << "SMF with " << tracks.size() << " track(s)" << endl
	    << mthd << endl;
	copy(
		tracks.begin(), tracks.end(),
		ostream_iterator<MTrk_t>(s,"\n") );
	s.flags(ff);
    }

}
