//
// hersheyfont.c - Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
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


#include "hersheyfont.h"

#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#ifndef HERSHEY_FONTS_DIR
// #define HERSHEY_FONTS_DIR "/usr/local/share/hershey-fonts"
#endif

#undef DEBUG


//
// Convert "Hershey-values" (offset by ASCII 'R') to integers
//
static inline short hershey_val(char c) { return (c - 'R'); }


//
// Load the (*hg) structure with data from the jhf_line.
//
static int
hershey_jhf_load_glyph( struct hershey_glyph *hg, char *jhf_line )
{
    char buf[8];
    char *end;

    unsigned int glyphnum, nverts;
    int leftpos, rightpos;
    char *vertchars;

    int len = strlen(jhf_line);
    if ( jhf_line[len-1] == '\n' ) {
	jhf_line[len-1] = 0;
	len--;
    }
    if ( len < 10 ) {
	fprintf(stderr, "%s: line length %d is too short\n", __FUNCTION__, len);
	errno = ERANGE;
	return 0;
    }

    errno = 0;

    strncpy(buf, jhf_line+0, 5);
    glyphnum = strtoul(buf, &end, 10);
    buf[5] = 0;
    if ( errno )
	return 0;

    strncpy(buf, jhf_line+5, 3);
    buf[3] = 0;
    nverts = strtoul(buf, &end, 10);
    if ( errno )
	return 0;

    len -= 8;

    if ( nverts*2 != len ) {
	fprintf(stderr, "%s: expected %d (not %d) coord bytes for nverts=%u\n", __FUNCTION__, nverts*2, len, nverts);
	errno = ERANGE;
	return 0;
    }

    leftpos = hershey_val(jhf_line[8]);
    rightpos = hershey_val(jhf_line[9]);
    if ( leftpos > rightpos ) {
	fprintf(stderr, "%s: bogus leftpos %d > rightpos %d\n", __FUNCTION__, leftpos, rightpos);
	errno = ERANGE;
	return 0;
    }

    // skip over the (leftpos,rightpos) pair
    nverts--;
    vertchars = jhf_line+10;

    // stuff the struct hershey_glyph

    hg->glyphnum = glyphnum;
    hg->width = rightpos - leftpos;	// FIXME add + 1 ??
    hg->npaths = 0;

    // split out the seperate line paths
    struct hershey_path *hp, *hplast=0;
    int i;
    int j = 0;
    for ( i=1; i<=nverts; i++ ) {
	// pen up
	if ( i==nverts || (vertchars[i*2]==' ' && vertchars[i*2+1]=='R') ) {

	    int npathverts = i - j;

	    unsigned long path_nbytes =
		    sizeof(struct hershey_path)
		    + npathverts * sizeof(struct hershey_vertex);

	    hp = malloc(path_nbytes);
#ifdef DEBUG
	    printf("... hp = malloc(%lu + %lu)\n",
		    sizeof(struct hershey_path),
		    npathverts * sizeof(struct hershey_vertex), hp );
#endif
	    assert(hp);

	    hp->next = 0;
	    hp->nverts = npathverts;
	    // Copy the vertices into hp->verts with y-invert and offsets to
	    // place the glyph origin at its lower left baseline corner with
	    // standard Cartesian coordinates.
	    int xoffset = -leftpos;	// shift left edge to 0
	    int yoffset = 16 - 7;	// (height - baseline) FIXME? hardcoded
	    int k;
	    for ( k=0; k<npathverts; k++ ) {
#ifdef DEBUG
		printf("... copy hp->verts[%d] = vertchars[%d*2]\n", k, k+j);
#endif
		hp->verts[k].x = xoffset + hershey_val(vertchars[(k+j)*2]);
		hp->verts[k].y = yoffset - hershey_val(vertchars[(k+j)*2 + 1]);
	    }

	    j = i + 1;

	    if ( hplast )
		hplast->next = hp;
	    else
		hg->paths = hp;
	    hg->npaths++;
	    hplast = hp;
	}
    }

    return 1;
}


//
// Load a Hershey font .jhf file into a new struct hershey_font.
//
struct hershey_font *
hershey_jhf_font_load( const char *jhffile )
{
    struct hershey_font *hf;

    FILE *fp = fopen(jhffile, "r");
    if (!fp)
	return 0;

    hf = calloc(1, sizeof(struct hershey_font));
    assert(hf);

#define BUFSIZE 4096

    char linebuf[BUFSIZE];
    int linecount = 0;
    int glyph_index = 32;

    while ( fgets(linebuf, BUFSIZE, fp) ) {
	int r;
	struct hershey_glyph *hg;
	hg = &(hf->glyphs[glyph_index++]);
	r = hershey_jhf_load_glyph(hg, linebuf);
	linecount++;
	if ( ! r ) {
	    perror(jhffile);
	    fprintf(stderr, "%s: ... at line number %d\n", jhffile, linecount);
	    hershey_font_free(hf);
	    hf = 0;
	    break;
	}
    }

    fclose(fp);

    return hf;
}


//
// Load Hershey fontname (e.g. "rowmans") into a new struct hershey_font.
// The associated .jhf font file will be loaded from .../share/hershey-fonts/.
//
struct hershey_font *
hershey_font_load( const char *fontname )
{
    const char *jhfdir;
    jhfdir = getenv("HERSHEY_FONTS_DIR");
    if ( !jhfdir )
	jhfdir = HERSHEY_FONTS_DIR;
    char *jhffile = malloc(strlen(jhfdir)+1+strlen(fontname)+5);
    assert(jhffile);
    sprintf(jhffile, "%s/%s.jhf", jhfdir, fontname);
    struct hershey_font *hf = hershey_jhf_font_load(jhffile);
    free(jhffile);
    return hf;
}


//
// Return a pointer to the struct hershey_glyph for ASCII character c.
//
struct hershey_glyph *
hershey_font_glyph( struct hershey_font *hf, unsigned char c )
{
    return &hf->glyphs[c];
}


//
// Free a hershey_font allocated by hershey_font_load or hershey_jhf_font_load.
//
void
hershey_font_free( struct hershey_font *hf )
{
    // free all the paths of each glyph
    int c;
    for ( c=0; c<256; c++ ) {
	const struct hershey_glyph *hg;
	struct hershey_path *hp, *next;
	hg = hershey_font_glyph(hf, c);
	for ( hp=hg->paths, next=hp; hp; hp=next ) {
	    next = hp->next;
	    free(hp);
	}
    }
    // free the hf structure itself
    free(hf);
}

