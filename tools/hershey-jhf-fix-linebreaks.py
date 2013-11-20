#!/usr/bin/python
#
# hershey-jhf-fix-linebreaks.py  -  fix linebreaks in .jhf Hershey font files
# Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

import sys

output = ''


def hershey_val(character):
    return ord(character) - ord('R')


def read_hershey_glyph(f):
    line = ''
    while ( line == '' ):
	line = f.readline()
	if (not line) or line == '\x1a':	# eof
	    return False
	line = line.rstrip()

    # read a Hershey format line
    glyphnum = int(line[0:5])		# glyphnum (junk in some .jhf files)
    nverts = int(line[5:8]) - 1
    leftpos = hershey_val(line[8])
    rightpos = hershey_val(line[9])
    vertchars = line[10:]
    nvertchars = len(vertchars)

    # join split lines in the Hershey data
    while ( nverts * 2 > nvertchars ):
	nextline = f.readline().rstrip()
	line += nextline
	vertchars += nextline
	nvertchars = len(vertchars)
    if ( nverts * 2 != nvertchars ):
	print >> sys.stderr, "hershey2olfont: Hershey format parse error (nvertchars=%d not %d)" % (nvertchars, nverts*2)
	sys.exit(1)

    # emit the fixed (joined) line
    global output
    output += "%s\n" % line

    return True


#
# main
#

if ( len(sys.argv) != 3 ):
    print >> sys.stderr, "usage: hershey-jhf-fix-linebreaks.py in.jhf out.jhf   (same filename is ok)"
    sys.exit(1)

hershey_in = sys.argv[1]
hershey_out = sys.argv[2]

with open(hershey_in) as f:
    while read_hershey_glyph(f):
	pass
    f.close()

fd = open(hershey_out,"w")
fd.write(output)
fd.close()

