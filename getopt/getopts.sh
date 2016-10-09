#!/bin/sh

#
# An example on getopts built in shell function. See also getopt.c which is a
# C program counterpart to this shell code.
#
# (c) jp@devnull.cz
#

usage="usage: `basename $0` command [-c code] [filename [filename [...]]]"

[ $# -lt 1 ] && echo "$usage" && exit 1
echo "first param (command): $0"

while getopts 'c:' c; do
	case ${c} in
	c)
		code="$OPTARG"
		echo "option -c set to '$code'"
		;;
	*)
		echo "$usage" && exit 1
		;;
	esac
done

echo "...done reading option arguments"

# -1 is for "shift 1" already done above
shift `expr $OPTIND - 1`

[ -n "$1" ] && echo "filenames: $*" || echo "no filenames entered"
