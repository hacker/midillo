#ifndef __MIDILLO_EXCEPTION_H
#define __MIDILLO_EXCEPTION_H

#include <konforka/exception.h>

/**
 * @file
 * @brief midillo specific exceptions
 */

namespace midillo {
    using std::string;

    /**
     * Base midillo exception class
     */
    class exception : public konforka::exception {
	public:
	    explicit exception(const string& fi,const string& fu,int l,const string& w)
		: konforka::exception(fi,fu,l,w) { }
    };

    class exception_io_error : public exception {
	public:
	    explicit exception_io_error(const string& fi,const string& fu,int l,const string& w)
		: exception(fi,fu,l,w) { }
    };

    class exception_input_error : public exception_io_error {
	public:
	    explicit exception_input_error(const string& fi,const string& fu,int l,const string& w)
		: exception_io_error(fi,fu,l,w) { }
    };

    class exception_invalid_input : public exception_input_error {
	public:
	    explicit exception_invalid_input(const string& fi,const string& fu,int l,const string& w)
		: exception_input_error(fi,fu,l,w) { }
    };

    class exception_unexpected_input : public exception_invalid_input {
	public:
	    explicit exception_unexpected_input(const string& fi,const string& fu,int l,const string& w)
		: exception_invalid_input(fi,fu,l,w) { }
    };

    class exception_output_error : public exception_io_error {
	public:
	    explicit exception_output_error(const string& fi,const string& fu,int l,const string& w)
		: exception_io_error(fi,fu,l,w) { }
    };

};

#endif /* __MIDILLO_EXCEPTION_H */
