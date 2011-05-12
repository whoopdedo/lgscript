/******************************************************************************
 * Copyright (C) 2011 Tom N Harris. All rights reserved.
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *  3. This notice may not be removed or altered from any source distribution.
 *  4. Neither the names of the authors nor the names of any of the software
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 */
#include <string.h>
#include <limits.h>

#define LISbit(B)	(1 << (B))

#define LISupper 	LISbit(0)
#define LISalpha 	LISbit(1)
#define LISdigit 	LISbit(2)
#define LISspace 	LISbit(3)
#define LIScntrl	LISbit(4)
#define LISpunct	LISbit(5)
#define LISxdigit	LISbit(6)

#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned char lc_ctype[UCHAR_MAX+2];
extern const short lc_tolower[UCHAR_MAX+2];
extern const short lc_toupper[UCHAR_MAX+2];
extern const short lc_collate[UCHAR_MAX+2];

extern int lstrcmp      (const char* s1, const char* s2);
extern int lstrcasecmp  (const char* s1, const char* s2);
extern int lstrncasecmp (const char* s1, const char* s2, size_t n);

#ifdef __cplusplus
}
#endif

#define lisctype(c, type)	((lc_ctype)[(int)(c)+1] & (type))

#define isalpha(c)	lisctype((c), LISalpha)
#define isalnum(c)	lisctype((c), LISalpha|LISdigit)
#define isdigit(c)	lisctype((c), LISdigit)
#define isspace(c)	lisctype((c), LISspace)
#define isprint(c)	(!lisctype((c), LIScntrl))
#define iscntrl(c)	lisctype((c), LIScntrl)
#define ispunct(c)	lisctype((c), LISpunct)
#define isxdigit(c)	lisctype((c), LISxdigit)
#define isupper(c)	lisctype((c), LISupper)
#define islower(c)	(lisctype((c), LISalpha|LISupper)==LISalpha)

#define toupper(c)	((lc_toupper)[(int)(c)+1])
#define tolower(c)	((lc_tolower)[(int)(c)+1])

#define strcasecmp	lstrcasecmp
#define strncasecmp	lstrncasecmp
#define strcoll 	lstrcmp
