//
// hershey-font-check.c - Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

#include <hersheyfont.h>
#include <stdio.h>
#include <string.h>


int
main( int argc, char **argv )
{
    if ( argc != 2 ) {
	fprintf(stderr,
		"usage: hershey-font-check {fontname | fontfile.jhf}\n");
	return 1;
    }

    // load a hershey_font structure either by filename or fontname
    const char *fontname = argv[1];
    struct hershey_font *hf;

    if ( strchr(fontname,'.') )
	hf = hershey_jhf_font_load(fontname);
    else
	hf = hershey_font_load(fontname);

    if ( !hf ) {
	perror(fontname);
	return 1;
    }

    // dump all the glyph data
    int c, i;
    for ( c=0; c<256; c++ ) {
	// get the hershey_glyph for ASCII character c
	struct hershey_glyph *hg = hershey_font_glyph(hf, c);

	// check whether there actually is a glyph for this character
	if ( hg->npaths == 0 )
	    continue;

	printf("  [[ %c ]] glyph(%d) width=%u npaths=%u\n",
			c, c, hg->width, hg->npaths);

	// walk the paths-list for this glyph
	struct hershey_path *hp;
	for ( hp=hg->paths; hp; hp=hp->next ) {
	    // begin draw path
	    printf("\t\tpath: nverts=%d\t", hp->nverts);
	    for ( i=0; i<hp->nverts; i++ ) {
		short x = hp->verts[i].x /* + x_render_pos */ ;
		short y = hp->verts[i].y;
		printf(" {%d,%d}", x, y);
	    }
	    // end draw path
	    printf("\n");
	}

	// note: to render a string of character glyphs left-to-right,
	// advance the x_render_pos by the width of each glyph here, e.g.:
	//
	// x_render_pos += hg->width;
    }

    // destroy the hershey_font
    hershey_font_free(hf);

    return 0;
}
