#include <midillo/util.h>
#include <midillo/exception.h>

namespace midillo {

    unsigned long read32(istream& s) {
	unsigned long rv = 0;
	for(int t=0;t<4;++t) {
	    rv<<=8;
	    rv|=0xFF&s.get();
	    if(!s.good())
		throw exception_input_error(CODEPOINT,"Input error");
	}
	return rv;
    }

    unsigned int read16(istream& s) {
	unsigned int rv = 0;
	for(int t=0;t<2;++t) {
	    rv<<=8;
	    rv|=0xFF&s.get();
	    if(!s.good())
		throw exception_input_error(CODEPOINT,"Input error");
	}
	return rv;
    }

    unsigned long readVL(istream& s) {
	unsigned long rv=s.get();
	if(!s.good())
	    throw exception_input_error(CODEPOINT,"Error reading VLQ");
	int b;
	if(rv&0x80) {
	    rv&=0x7F;
	    do {
		rv = (rv<<7)|( (b=s.get())&0x7F );
		if(!s.good())
		    throw exception_input_error(CODEPOINT,"Error reading VLQ");
	    }while(b&0x80);
	}
	return rv;
    }

    void write32(ostream& s,unsigned long d) {
	for(int t=3;t>=0;--t) {
	    s.put(((const char*)&d)[t]);
	    if(!s.good())
		throw exception_output_error(CODEPOINT,"Output error");
	}
    }

    void write16(ostream& s,unsigned int d) {
	for(int t=1;t>=0;--t) {
	    s.put(((const char*)&d)[t]);
	    if(!s.good())
		throw exception_output_error(CODEPOINT,"Output error");
	}
    }

    void writeVL(ostream& s,unsigned long d) {
	// TODO: I don't think this is perfectly written code
	unsigned long tmp = d&0x7F;
	while(d>>=7) {
	    tmp<<=8;
	    tmp |=((d&0x7F)|0x80);
	}
	for(;;) {
	    s.put(tmp&0xFF);
	    if(!s.good())
		throw exception_output_error(CODEPOINT,"Error writing VLQ");
	    if(tmp&0x80)
		tmp>>=8;
	    else
		break;
	}
    }

    unsigned long calcVLsize(unsigned long d) {
	unsigned long rv = 0;
	// TODO: imperfect code revisited
	unsigned long tmp = d&0x7F;
	while(d>>=7) {
	    tmp<<=8;
	    tmp |=((d&0x7F)|0x80);
	}
	for(;;) {
	    ++rv;
	    if(tmp&0x80)
		tmp>>=8;
	    else
		break;
	}
	return rv;
    }

}
