#!/bin/sh
srcfile="${1}"
enumfile="${2}"
deffile="${3}"
IFS=':'
exec 5>${enumfile}
exec 6>${deffile}
SEQN=0
LN=0
cat ${srcfile} \
    | while read id ids help ; do
	LN="`expr ${LN} + 1`"
    	test -z "${id}" && continue
	test "${id}" != "${id# }" && continue
	echo "#line ${LN} \"${srcfile}\"" >&5
	echo " filter_${id} = ${SEQN}," >&5
	echo "#line ${LN} \"${srcfile}\"" >&6
	echo ' { { "'$(echo $ids|sed -e 's-/-", "-g')'", 0 }, "'"${help}"'", false },' >&6
	SEQN="`expr ${SEQN} + 1`"
    done
