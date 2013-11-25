//
// hersheyfont.h - Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
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

#ifndef HERSHEYFONT_H
#define HERSHEYFONT_H

struct hershey_vertex {
	short			x;
	short			y;
};

struct hershey_path {
	struct hershey_path	*next;
	unsigned short		nverts;
	struct hershey_vertex	verts[];
};

struct hershey_glyph {
	unsigned int		glyphnum;
	unsigned short		width;
	unsigned short		npaths;
	struct hershey_path	*paths;
};

struct hershey_font {
	struct hershey_glyph	glyphs[256];	/* indexed by ASCII value */
};


//
// Load Hershey fontname (e.g. "rowmans") into a new struct hershey_font.
// The associated .jhf font file will be loaded from .../share/hershey-fonts/.
//
struct hershey_font *	hershey_font_load( const char *fontname );


//
// Load a Hershey font .jhf file into a new struct hershey_font.
//
struct hershey_font *	hershey_jhf_font_load( const char *jhffile );

//
// Return a pointer to the struct hershey_glyph for ASCII character c.
//
struct hershey_glyph *	hershey_font_glyph( struct hershey_font *hf,
					    unsigned char c );

//
// Free a hershey_font allocated by hershey_font_load or hershey_jhf_font_load.
//
void			hershey_font_free( struct hershey_font *hf );

#endif // HERSHEYFONT_H
