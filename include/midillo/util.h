#ifndef __MIDILLO_UTIL_H
#define __MIDILLO_UTIL_H

#include <istream>
#include <ostream>

/**
 * @file
 * @brief utilities
 */

namespace midillo {
    using std::istream;
    using std::ostream;

    /**
     * read 32 bits word from the stream
     * @param s input stream
     * @return the data acquired
     */
    unsigned long read32(istream& s);
    /**
     * read 16 bits word from the stream
     * @param s input stream
     * @return the data acquired
     */
    unsigned int read16(istream& s);
    /**
     * read the variable length quantity from the stream
     * @param s input stream
     * @return the data acquired
     */
    unsigned long readVL(istream& s);

    /**
     * write 32 bits word to the stream
     * @param s output stream
     * @param d data to write
     */
    void write32(ostream& s,unsigned long d);
    /**
     * write 16 bits word to the stream
     * @param s output stream
     * @param d data to write
     */
    void write16(ostream& s,unsigned int d);
    /**
     * write the variable length quantity to the stream
     * @param s output stream
     * @param d data to write
     */
    void writeVL(ostream& s,unsigned long d);

    /**
     * calculate the amount of data that would be written by writeVL
     * @param d data that would be written
     * @return the number of bytes
     */
    unsigned long calcVLsize(unsigned long d);

}

#endif /* __MIDILLO_UTIL_H */
