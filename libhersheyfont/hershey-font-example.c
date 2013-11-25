//
// hershey-font-example.c - Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
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


int
main( int argc, char **argv )
{
    const char *render_text = "HELLO";

    //
    // Load a Hershey font with hershey_font_load().
    //

    const char *fontname = "rowmans";
    struct hershey_font *hf = hershey_font_load(fontname);
    if ( !hf ) {
	perror(fontname);
	return 1;
    }

    //
    // Render each character's line paths with hershey_font_glyph().
    //

    int x_render_pos = 0;

    const char *p;
    for ( p=render_text; *p; p++ ) {

	// get the character c to be rendered
	int c = *p;

	// get the hershey_glyph for character c
	// and check whether there actually is a glyph for this character
	struct hershey_glyph *hg = hershey_font_glyph(hf, c);
	if ( hg->width == 0 )
	    continue;

	printf("# %c\n", c);

	// walk the paths-list for this glyph
	struct hershey_path *hp;
	for ( hp=hg->paths; hp; hp=hp->next ) {
	    // begin draw path
	    printf("\t");
	    int i;
	    for ( i=0; i<hp->nverts; i++ ) {
		short x = hp->verts[i].x + x_render_pos;
		short y = hp->verts[i].y;
		printf(" {%d,%d}", x, y);
	    }
	    // end draw path
	    printf("\n");
	}

	// advance the x_render_pos by the width of this glyph
	x_render_pos += hg->width;
    }

    //
    // Destroy the hershey_font with hershey_font_free().
    //
    hershey_font_free(hf);

    return 0;
}
