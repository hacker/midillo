#include <midillo/chunk.h>
#include <midillo/util.h>
#include <midillo/exception.h>

namespace midillo {

    void chunk_header_t::load(istream& s) {
	s.read((char*)id_chars,sizeof(id_chars));
	if(!s.good())
	    throw exception_input_error(CODEPOINT,"Error reading chunk header");
	length = read32(s);
    }
    
    void chunk_header_t::save(ostream& s) const {
	s.write((char*)id_chars,sizeof(id_chars));
	if(!s.good())
	    throw exception_output_error(CODEPOINT,"Error writing chunk header");
	write32(s,length);
    }

    void chunk_header_t::dump(ostream& s) const {
	std::ios::fmtflags ff = s.flags();
	s.unsetf(std::ios::hex); s.setf(std::ios::dec);
	s
	    << id_chars[0] << id_chars[1]
	    << id_chars[2] << id_chars[3]
	    << " chunk of " << length << " byte(s)";
	s.flags(ff);
    }

}
