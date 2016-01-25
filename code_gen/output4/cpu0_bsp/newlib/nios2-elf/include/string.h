/*
 * string.h
 *
 * Definitions for memory and string functions.
 */

#ifndef _STRING_H_
#define	_STRING_H_

#include "_ansi.h"
#include <sys/reent.h>

#define __need_size_t
#include <stddef.h>

#ifndef NULL
#define NULL 0
#endif

_BEGIN_STD_C

_PTR 	 _EXFUN(memchr,(const _PTR, int, size_t));
int 	 _EXFUN(memcmp,(const _PTR, const _PTR, size_t));
_PTR 	 _EXFUN(memcpy,(_PTR, const _PTR, size_t));
_PTR	 _EXFUN(memmove,(_PTR, const _PTR, size_t));
_PTR	 _EXFUN(memset,(_PTR, int, size_t));
char 	*_EXFUN(strcat,(char *, const char *));
char 	*_EXFUN(strchr,(const char *, int));
int	 _EXFUN(strcmp,(const char *, const char *));
int	 _EXFUN(strcoll,(const char *, const char *));
char 	*_EXFUN(strcpy,(char *, const char *));
size_t	 _EXFUN(strcspn,(const char *, const char *));
char 	*_EXFUN(strerror,(int));
size_t	 _EXFUN(strlen,(const char *));
char 	*_EXFUN(strncat,(char *, const char *, size_t));
int	 _EXFUN(strncmp,(const char *, const char *, size_t));
char 	*_EXFUN(strncpy,(char *, const char *, size_t));
char 	*_EXFUN(strpbrk,(const char *, const char *));
char 	*_EXFUN(strrchr,(const char *, int));
size_t	 _EXFUN(strspn,(const char *, const char *));
char 	*_EXFUN(strstr,(const char *, const char *));

#ifndef _REENT_ONLY
char 	*_EXFUN(strtok,(char *, const char *));
#endif

size_t	 _EXFUN(strxfrm,(char *, const char *, size_t));

#ifndef __STRICT_ANSI__
char 	*_EXFUN(strtok_r,(char *, const char *, char **));

int	 _EXFUN(bcmp,(const void *, const void *, size_t));
void	 _EXFUN(bcopy,(const void *, void *, size_t));
void	 _EXFUN(bzero,(void *, size_t));
int	 _EXFUN(ffs,(int));
char 	*_EXFUN(index,(const char *, int));
_PTR	 _EXFUN(memccpy,(_PTR, const _PTR, int, size_t));
_PTR	 _EXFUN(mempcpy,(_PTR, const _PTR, size_t));
_PTR	 _EXFUN(memmem, (const _PTR, size_t, const _PTR, size_t));
char 	*_EXFUN(rindex,(const char *, int));
char 	*_EXFUN(stpcpy,(char *, const char *));
char 	*_EXFUN(stpncpy,(char *, const char *, size_t));
int	 _EXFUN(strcasecmp,(const char *, const char *));
char	*_EXFUN(strcasestr,(const char *, const char *));
char 	*_EXFUN(strdup,(const char *));
char 	*_EXFUN(_strdup_r,(struct _reent *, const char *));
char 	*_EXFUN(strndup,(const char *, size_t));
char 	*_EXFUN(_strndup_r,(struct _reent *, const char *, size_t));
char 	*_EXFUN(strerror_r,(int, char *, size_t));
size_t	 _EXFUN(strlcat,(char *, const char *, size_t));
size_t	 _EXFUN(strlcpy,(char *, const char *, size_t));
int	 _EXFUN(strncasecmp,(const char *, const char *, size_t));
size_t	 _EXFUN(strnlen,(const char *, size_t));
char 	*_EXFUN(strsep,(char **, const char *));
char	*_EXFUN(strlwr,(char *));
char	*_EXFUN(strupr,(char *));
#ifdef __CYGWIN__
#ifndef DEFS_H	/* Kludge to work around problem compiling in gdb */
const char  *_EXFUN(strsignal, (int __signo));
#endif
int     _EXFUN(strtosigno, (const char *__name));
#endif

/* These function names are used on Windows and perhaps other systems.  */
#ifndef strcmpi
#define strcmpi strcasecmp
#endif
#ifndef stricmp
#define stricmp strcasecmp
#endif
#ifndef strncmpi
#define strncmpi strncasecmp
#endif
#ifndef strnicmp
#define strnicmp strncasecmp
#endif

#endif /* ! __STRICT_ANSI__ */

/* Inline versions of common functions. */
#ifdef __NIOS2__
#ifndef __cplusplus
#ifdef __OPTIMIZE__
#ifndef __OPTIMIZE_SIZE__

#ifdef WANT_INLINE_STRCMP
extern __inline__ int __attribute__ ((always_inline))
strcmp (const char *s1, const char *s2)
{

  /* If s1 or s2 are unaligned, then compare bytes. */
  if (!(((long)s1 & (sizeof (long) - 1)) | ((long)s2 & (sizeof (long) - 1))))
    {
      long *a1;
      long *a2;
      long d1, d2;
      long m1, m2;

      /* If s1 and s2 are word-aligned, compare them a word at a time. */
      a1 = (unsigned long*)s1;
      a2 = (unsigned long*)s2;
      d1 = *a1++;
      d2 = *a2++;
      while (d1 == d2)
        {
          /* To get here, *a1 == *a2, thus if we find a null in *a1,
             then the strings must be equal, so return zero.  */
          if (((d1) - 0x01010101) & ~(d1) & 0x80808080)
            return 0;

          d1 = *a1++;
          d2 = *a2++;
        }

      /* the 32-bit values are different, therefore,
         we need to find the byte that differed.
         Remeber: the difference could be after
         the NUL byte, therefore making the strings 
         equal */

#ifdef __nios2_big_endian__
      m1 = d1 & 0xffffff00;
      m2 = d2 & 0xffffff00;
      if (!(((d1) - 0x01010101) & ~(d1) & 0x80808080) && m1 == m2)
        return d1 - d2;

      m1 = d1 & 0xff000000;
      m2 = d2 & 0xff000000;
      if (m1 == 0 || m1 != m2)
        return m1 - m2;

      m1 = d1 & 0xff0000;
      m2 = d2 & 0xff0000;
      if (m1 == 0 || m1 != m2)
        return m1 - m2;

      m1 = d1 & 0xff00;
      m2 = d2 & 0xff00;
      if (m1 == 0 || m1 != m2)
        return m1 - m2;

      /* the difference is in the last byte,
         therefore the lower three bytes are equal
         and do not need to be masked out */
      return d1 - d2;
#else
      m1 = d1 & 0xffffff;
      m2 = d2 & 0xffffff;
      if (!(((d1) - 0x01010101) & ~(d1) & 0x80808080) && m1 == m2)
        return d1 - d2;

      m1 = d1 & 0xff;
      m2 = d2 & 0xff;
      if (m1 == 0 || m1 != m2)
        return m1 - m2;

      m1 = d1 & 0xff00;
      m2 = d2 & 0xff00;
      if (m1 == 0 || m1 != m2)
        return m1 - m2;

      m1 = d1 & 0xff0000;
      m2 = d2 & 0xff0000;
      if (m1 == 0 || m1 != m2)
        return m1 - m2;

      /* the difference is in the last byte,
         therefore the lower three bytes are equal
         and do not need to be masked out */
      return d1 - d2;
#endif
    }

  while (*s1 != '\0' && *s1 == *s2)
    {
      s1++;
      s2++;
    }
  return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}
#endif


#endif
#endif
#endif
#endif


#include <sys/string.h>

_END_STD_C

#endif /* _STRING_H_ */
