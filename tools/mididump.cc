#include <getopt.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
#include <konforka/exception.h>
#include <midillo/SMF.h>
using namespace midillo;

#include "config.h"
#define PHEADER PACKAGE " " VERSION " - mididump - dump midi files"
#define PCOPY "Copyright (c) 2006 Klever Group"

static void usage(const char *p) {
    cerr << PHEADER << endl
	<< PCOPY << endl << endl
	<< " " << p << " [options] [<input-file>[ <output-file>]]" << endl << endl
	<< " -h, --help" << endl
	<< " --usage          display this text" << endl
	<< " -V, --version    display version number" << endl
	<< " -L, --license    show license" << endl;
}

main(int argc,char **argv) {
    try {
	while(true) {
	    static struct option opts[] = {
		{ "help", no_argument, 0, 'h' },
		{ "usage", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V' },
		{ "license", no_argument, 0, 'L' },
		{ NULL, 0, 0, 0 }
	    };
	    int c = getopt_long(argc,argv,"f:hVLl",opts,NULL);
	    if(c==-1)
		break;
	    switch(c) {
		case 'h':
		    usage(*argv);
		    exit(0);
		    break;
		case 'V':
		    cerr << VERSION << endl;
		    exit(0);
		    break;
		case 'L':
		    extern const char *COPYING;
		    cerr << COPYING << endl;
		    exit(0);
		    break;
		default:
		    cerr << "Huh??" << endl;
		    break;
	    }
	}
	const char *infile = "-";
	if(optind<argc)
	    infile = argv[optind++];
	const char *oufile = "-";
	if(optind<argc)
	    oufile = argv[optind++];
	if(optind<argc) {
	    usage(*argv);
	    exit(1);
	}
	SMF_t in(infile);
	if(strcmp(oufile,"-")) {
	    ofstream s(oufile); s << in;
	}else{
	    cout << in;
	}
	return 0;
    }catch(konforka::exception& e) {
	cerr << "Oops... Konforka exception:" << endl
	    << " what: " << e.what() << endl
	    << " where: " << e.where() << endl;
	return 1;
    }catch(exception& e) {
	cerr << "Oops... Exception:" << endl
	    << " what: " << e.what() << endl;
	return 1;
    }
}
