lib_LTLIBRARIES = libmidillo.la

INCLUDES = -I${top_srcdir}/include -I${top_srcdir}
AM_CXXFLAGS = ${KONFORKA_CFLAGS}
LDADD = ${KONFORKA_LIBS}

libmidillo_la_SOURCES = \
    util.cc \
    SMF.cc \
    chunk.cc \
    MThd.cc MTrk.cc \
    event.cc \
    message.cc

libmidillo_la_LDFLAGS = -version-info 0:0:0
