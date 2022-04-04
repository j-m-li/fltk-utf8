// Standard dialog header file for the UTF-8 Fast Light Tool Kit (FLTK-UTF8).
//
// Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version
// with exceptions that allow sub-classing and static linking in
// non-LGPL compliant software. These exceptions are subject to
// conditions, see the FLTK License for more details.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the FLTK
// License for more details.
//
// You should have received a copy of the FLTK License along with
// this library; if not, write to  OksiD Software, Jean-Marc Lienher,
// Rue de la Cheminee 1, CH-2065 Savagnier, Switzerland.
//
// Please report all bugs and problems to "oksid@bluewin.ch".
//

/*** NOTE : all functions are LIMITED to 24 bits Unicode values !!! ***/

#ifndef fl_utf8_h
#  define fl_utf8_h
#  include <stdio.h>
#  include <string.h>
#  include <stdlib.h>

#  ifndef FL_EXPORT
#    if defined(FL_DLL) && defined(_MSC_VER)
#      ifdef FL_LIBRARY
#        define FL_EXPORT   __declspec(dllexport)
#      else
#        define FL_EXPORT   __declspec(dllimport)
#      endif /* FL_LIBRARY */
#    else
#      define FL_EXPORT
#   endif /* FL_DLL && _MSC_VER */
#  endif /* FL_EXPORT */



#  if __APPLE__
#    include <wchar.h>
#    include <stat.h>
#    define xchar wchar_t
#  else
#    include <sys/types.h>
#    include <sys/stat.h>
#    ifndef _WIN32 
#      include "Xutf8.h"
#    include <X11/Xlocale.h>
#    include <X11/Xlib.h>
#    endif
#    include <locale.h>
#  endif
#  if defined(WIN32)
#    include <ctype.h>
#    define xchar wchar_t
#  else
#    define xchar unsigned short
#  endif

#  if defined(WIN32) && !defined(FL_DLL)
#    undef strdup 
#    define strdup _strdup
#    undef putenv
#    define putenv _putenv
#    undef stricmp
#    define stricmp _stricmp
#    undef strnicmp
#    define strnicmp _strnicmp
#    undef hypot
#    define hypot _hypot
#    undef chdir
#    define chdir _chdir
#  endif

#  ifdef __cplusplus
extern "C" {
#  endif

FL_EXPORT int fl_utf2ucs( const unsigned char *buf, int len, unsigned int *ucs);
FL_EXPORT int fl_ucs2utf(unsigned int ucs, char *buf);
FL_EXPORT int fl_utflen(const unsigned char *buf, int len);
FL_EXPORT int fl_utf_nb_char(const unsigned char *buf, int len);
FL_EXPORT int fl_utf_strncasecmp(const char *s1, const char *s2, int n);
FL_EXPORT int fl_utf_strcasecmp(const char *s1, const char *s2);
FL_EXPORT int fl_tolower(unsigned int ucs);
FL_EXPORT int fl_toupper(unsigned int ucs);
FL_EXPORT int fl_utf_tolower(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_utf_toupper(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_utf2unicode(const unsigned char *str, int len, xchar *buf);
FL_EXPORT int fl_unicode2utf(const xchar *str, int len, char *buf);
FL_EXPORT int fl_utf2latin1(const unsigned char *str, int len, char *buf);
FL_EXPORT int fl_latin12utf(const unsigned char *str, int len, char *buf);
FL_EXPORT unsigned int fl_nonspacing(unsigned int ucs);
FL_EXPORT char* fl_utf2mbcs(const char * s);
FL_EXPORT char* fl_mbcs2utf(const char *s);

FL_EXPORT int fl_is_nt4(void);
FL_EXPORT int fl_chmod(const char* f, int mode);
FL_EXPORT int fl_access(const char* f, int mode);
FL_EXPORT int fl_stat( const char *path, struct stat *buffer );
FL_EXPORT char* fl_getcwd( char *buf, int maxlen);
FL_EXPORT FILE *fl_fopen(const char *f, const char *mode);
FL_EXPORT int fl_system(const char* f);
FL_EXPORT int fl_execvp(const char *file, char *const *argv);
FL_EXPORT int fl_open(const char* f, int o, ...);
FL_EXPORT int fl_unlink(const char *f);
FL_EXPORT int fl_rmdir(const char *f);
FL_EXPORT char* fl_getenv(const char *name);
FL_EXPORT int fl_mkdir(const char* f, int mode);
FL_EXPORT int fl_rename(const char* f, const char *t);

FL_EXPORT void fl_make_path_for_file( const char *path );
FL_EXPORT char fl_make_path( const char *path );

#ifdef WIN32
FL_EXPORT char *fl_utf82locale(const char *s, int len, unsigned int codepage);
FL_EXPORT char *fl_locale2utf8(const char *s, int len, unsigned int codepage);
#endif

#  ifdef __cplusplus
}
#  endif

#endif
 
//
// End of "$Id: $".
//

