/******************************************************************************
 *    strtocolor.cpp
 *
 *    This file is part of LgScript
 *    Copyright (C) 2009 Tom N Harris <telliamed@whoopdedo.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 *****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <lua/lctype.h>

static const unsigned char g_cHexTable[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,
	0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

typedef struct _colorname
{
	const char* n;
	unsigned long c;
} colorname;
static const colorname g_sColorNames[] = {
	{ "aqua",    0xFFFF00 },
	{ "black",   0x080808 },
	{ "blue",    0xFF0000 },
	{ "chartreuse", 0x80FF80 },
	{ "dodger",     0x0080FF },
	{ "fuchsia", 0xFF00FF },
	{ "gold",       0x80FFFF },
	{ "gray",    0x808080 },
	{ "green",   0x008000 },
	{ "grey",    0x808080 },
	{ "lawn",       0x00FF80 },
	{ "lime",    0x00FF00 },
	{ "maroon",  0x000080 },
	{ "navy",    0x800000 },
	{ "olive",   0x008080 },
	{ "orange",     0x0080FF },
	{ "orchid",     0xFF80FF },
	{ "pink",       0x8000FF },
	{ "purple",  0x800080 },
	{ "red",     0x0000FF },
	{ "rose",       0x8080FF },
	{ "silver",  0xC0C0C0 },
	{ "slate",      0xFF8080 },
	{ "spring",     0x80FF00 },
	{ "steel",      0xFF8000 },
	{ "teal",    0x808000 },
	{ "turquoise",  0xFFFF80 },
	{ "violet",     0xFF0080 },
	{ "white",   0xFFFFFF },
	{ "yellow",  0x00FFFF }
};
static const unsigned int g_iNumColors = sizeof(g_sColorNames)/sizeof(g_sColorNames[0]);

static int colornamecmp(const void* key, const void* elem)
{
	return l_strcasecmp(reinterpret_cast<const char*>(key),
		reinterpret_cast<const colorname*>(elem)->n);
}

int strtocolor(const char* str)
{
	register const unsigned char* c = reinterpret_cast<const unsigned char*>(str);
	if (*c == '#')
	{
		if (strlen(reinterpret_cast<const char*>(++c)) < 6)
			return 0;
		unsigned char r,g,b;
		r  = g_cHexTable[*c++] << 4;
		r |= g_cHexTable[*c++];
		g  = g_cHexTable[*c++] << 4;
		g |= g_cHexTable[*c++];
		b  = g_cHexTable[*c++] << 4;
		b |= g_cHexTable[*c];
		return (r|(g<<8)|(b<<16));
	}
	if (strchr(str, ','))
	{
		unsigned int r,g,b;
		if (3 == sscanf(str, "%u , %u , %u", &r, &g, &b))
		{
			return ((r&0xFF)|((g&0xFF)<<8)|((b&0xFF)<<16));
		}
	}
	else
	{
		const colorname* clr = reinterpret_cast<const colorname*>(
			bsearch(reinterpret_cast<const void*>(str),
				reinterpret_cast<const void*>(g_sColorNames),
				g_iNumColors,
				sizeof(colorname),
				colornamecmp)
			);
		if (clr)
			return clr->c;
	}
	return 0;
}
