/******************************************************************************
 *  utils.h
 *
 *  This file is part of LgScript
 *  Copyright (C) 2011 Tom N Harris <telliamed@whoopdedo.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************/
#ifndef LGUTILS_H
#define LGUTILS_H

int strtocolor(const char* str);

inline ulong makecolor(unsigned char r, unsigned char g, unsigned char b)
{
        return ((ulong)r) | ((ulong)g << 8) | ((ulong)b << 16);
}
inline unsigned char getred(ulong c) { return (unsigned char)(c & 0xFF); }
inline unsigned char getgreen(ulong c) { return (unsigned char)(c >> 8 & 0xFF); }
inline unsigned char getblue(ulong c) { return (unsigned char)(c >> 16 & 0xFF); }

#endif
