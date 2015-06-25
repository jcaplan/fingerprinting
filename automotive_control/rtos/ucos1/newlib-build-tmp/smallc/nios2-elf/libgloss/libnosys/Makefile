# Copyright (c) 1998 Cygnus Support
#
# The authors hereby grant permission to use, copy, modify, distribute,
# and license this software and its documentation for any purpose, provided
# that existing copyright notices are retained in all copies and that this
# notice is included verbatim in any distributions. No written agreement,
# license, or royalty fee is required for any of the authorized uses.
# Modifications to this software may be copyrighted by their authors
# and need not follow the licensing terms described here, provided that
# the new terms are clearly indicated on the first page of each file where
# they apply.

DESTDIR =
VPATH = /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/libnosys
srcdir = /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/libnosys
objdir = .
srcroot = $(srcdir)/../..
objroot = $(objdir)/../..

prefix = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib
exec_prefix = ${prefix}

host_alias = nios2-elf
target_alias = nios2-elf
program_transform_name = s,^,nios2-elf-,

bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
tooldir = $(exec_prefix)/$(target_alias)

# Multilib support variables.
# TOP is used instead of MULTI{BUILD,SRC}TOP.
MULTIDIRS = 
MULTISUBDIR = 
MULTIDO = true
MULTICLEAN = true

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

SHELL =	/bin/sh

CC = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/build-newlib-stage-wrapper nios2-elf-gcc -B/data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/smallc/nios2-elf/newlib/ -isystem /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/smallc/nios2-elf/newlib/targ-include -isystem /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/newlib/libc/include -B/data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/smallc/nios2-elf/libgloss/nios2 -L/data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/smallc/nios2-elf/libgloss/libnosys -L/CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/nios2

#AS = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/build-newlib-stage-wrapper nios2-elf-as
AS = `if [ -f ${objroot}/../gas/as-new ] ; \
	then echo ${objroot}/../gas/as-new ; \
	else echo as ; fi`

AR = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/build-newlib-stage-wrapper nios2-elf-ar

#LD = nios2-elf-ld
LD = `if [ -f ${objroot}/../ld/ld-new ] ; \
	then echo ${objroot}/../ld/ld-new ; \
	else echo ld ; fi`

RANLIB = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/build-newlib-stage-wrapper nios2-elf-ranlib

OBJDUMP = `if [ -f ${objroot}/../binutils/objdump ] ; \
	then echo ${objroot}/../binutils/objdump ; \
	else t='$(program_transform_name)'; echo objdump | sed -e $$t ; fi`
OBJCOPY = `if [ -f ${objroot}/../binutils/objcopy ] ; \
	then echo ${objroot}/../binutils/objcopy ; \
	else t='$(program_transform_name)'; echo objcopy | sed -e $$t ; fi`

# object files needed
OBJS = chown.o close.o environ.o errno.o execve.o fork.o fstat.o \
	getpid.o gettod.o isatty.o kill.o link.o lseek.o open.o \
	read.o readlink.o sbrk.o stat.o symlink.o times.o unlink.o \
	wait.o write.o _exit.o

# Object files specific to particular targets.
EVALOBJS = ${OBJS}

GCC_LDFLAGS = `if [ -d ${objroot}/../gcc ] ; \
	then echo -L${objroot}/../gcc ; fi`

OUTPUTS = libnosys.a

NEWLIB_CFLAGS = `if [ -d ${objroot}/newlib ]; then echo -I${objroot}/newlib/targ-include -I${srcroot}/newlib/libc/include; fi`
NEWLIB_LDFLAGS = `if [ -d ${objroot}/newlib ]; then echo -B${objroot}/newlib/ -L${objroot}/newlib/; fi`

INCLUDES = -I. -I$(srcdir)/..
# Note that when building the library, ${MULTILIB} is not the way multilib
# options are passed; they're passed in $(CFLAGS).
CFLAGS_FOR_TARGET = ${MULTILIB} ${INCLUDES} ${NEWLIB_CFLAGS}
LDFLAGS_FOR_TARGET = ${MULTILIB} ${NEWLIB_LDFLAGS}
AR_FLAGS = qc

.c.o:
	$(CC) $(CFLAGS_FOR_TARGET) -O2 $(INCLUDES) -c $(CFLAGS) $<

.C.o:
	$(CC) $(CFLAGS_FOR_TARGET) -O2 $(INCLUDES) -c $(CFLAGS) $<
.s.o:
	$(AS) $(ASFLAGS_FOR_TARGET) $(INCLUDES) $(ASFLAGS) -o $*.o $<

#
# GCC knows to run the preprocessor on .S files before it assembles them.
#
.S.o:
	$(CC) $(CFLAGS_FOR_TARGET) $(INCLUDES) $(CFLAGS) -c $<

#
# this is a bogus target that'll produce an assembler from the
# C source with the right compiler options. this is so we can
# track down code generation or debug symbol bugs.
#
.c.s:
	$(CC) $(CFLAGS_FOR_TARGET) -S $(INCLUDES) $(CFLAGS) $<

all: ${OUTPUTS}

#
# here's where we build the library for each target
#

libnosys.a: $(EVALOBJS)
	${AR} ${ARFLAGS} $@ $(EVALOBJS)
	${RANLIB} $@

doc:	

clean mostlyclean:
	rm -f $(OUTPUTS) *.i *~ *.o *-test *.srec *.dis *.map *.x

distclean maintainer-clean realclean: clean
	rm -f Makefile config.status $(OUTPUTS)

.PHONY: install info install-info clean-info
install:
	@for outputs in ${OUTPUTS}; do\
	 mkdir -p $(DESTDIR)$(tooldir)/lib${MULTISUBDIR}; \
	 $(INSTALL_PROGRAM) $${outputs} $(DESTDIR)$(tooldir)/lib${MULTISUBDIR}; \
	done

info:
install-info:
clean-info:

Makefile: Makefile.in config.status /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/libnosys/../config/default.mh
	$(SHELL) config.status

config.status: configure
	$(SHELL) config.status --recheck
