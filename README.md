hershey-fonts : Hershey vector fonts library, tools, and glyphs
===============================================================


Project home page: <http://www.whence.com/hershey-fonts/>
---------------------------------------------------------

  - Kamal Mostafa <kamal@whence.com>
  - Dr. A. V. Hershey
  - James Hurt


Package components
------------------

### libhersheyfont and hershey-font apps

Kamal Mostafa's **libhersheyfont** (a C library for loading .jhf format
Hershey font files) along with various utility programs such as
**hershey-font-gnuplot** (a Hershey text front-end for _gnuplot_).

### hershey-fonts-data

The Hershey fonts are a collection of vector fonts developed circa 1967
by Dr. A. V. Hershey.  Included are Latin, Greek, Cyrillic, Japanese, and
various symbol glyph sets encoded as .jhf format Hershey font files.
The .jhf font files were converted from Hershey's original NTIS format
files by James Hurt (see hershey-fonts.notes).
<http://en.wikipedia.org/wiki/Hershey_font>.


Build and install instructions
------------------------------

	cmake .
	make
	make test
	sudo make install     # optional

### The `HERSHEY_FONTS_DIR` environment variable

By default, **libhersheyfont** finds .jhf font files installed into the
system directory /usr/share/hershey-fonts/ or /usr/local/share/hershey-fonts/
depending on the build configuration.

To use **libhersheyfont** apps without installing, set the `HERSHEY_FONTS_DIR`
environment variable, e.g.:

	export HERSHEY_FONTS_DIR=$PWD/hershey-fonts
	tools/hershey-font-check futural
	tools/hershey-font-gnuplot . . .


License
-------

The **libhersheyfont** library and apps are
Copyright © 2013 Kamal Mostafa <kamal@whence.com>, licensed GPLv2+:
GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>.
The Hershey font glyph data is covered by a permissive use and redistribution
license.  This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

