bin_PROGRAMS = midi2f0 midifilter mididump

INCLUDES = -I${top_srcdir}/include ${KONFORKA_CFLAGS}
LIBS += ${top_builddir}/lib/libmidillo.la ${KONFORKA_LIBS}

midi2f0_SOURCES = midi2f0.cc \
    COPYING.cc
midi2f0_DEPENDENCIES = ${top_builddir}/lib/libmidillo.la

midifilter_SOURCES = midifilter.cc \
    COPYING.cc \
    filters.list \
    enflesh_filters_list.sh
midifilter_DEPENDENCIES = ${top_builddir}/lib/libmidillo.la
${srcdir}/midifilter.cc: filters_enumeration.cc filters_definition.cc
filters_enumeration.cc filters_definition.cc: enflesh_filters_list.sh filters.list 
	sh $^ \
	    filters_enumeration.cc filters_definition.cc

mididump_SOURCES = mididump.cc \
    COPYING.cc
mididump_DEPENDENCIES = ${top_builddir}/lib/libmidillo.la

clean-local:
	rm -f filters_enumeration.cc filters_definition.cc


COPYING.cc: ${top_srcdir}/COPYING
	echo "const char * COPYING =" >$@ || (rm $@;exit 1)
	sed 's/"/\\"/g' $< | sed 's/^/\"/' | sed 's/$$/\\n\"/' >>$@ || (rm $@;exit 1)
	echo ";" >>$@ || (rm $@;exit 1)
