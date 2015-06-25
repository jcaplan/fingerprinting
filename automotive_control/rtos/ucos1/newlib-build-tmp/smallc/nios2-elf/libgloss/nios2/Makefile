#   
#   Copyright (c) 2003 Altera Corporation
#   All rights reserved.
#   
#   Redistribution and use in source and binary forms, with or without modification,
#   are permitted provided that the following conditions are met:
#   
#      o Redistributions of source code must retain the above copyright notice, 
#        this list of conditions and the following disclaimer. 
#      o Redistributions in binary form must reproduce the above copyright notice, 
#        this list of conditions and the following disclaimer in the documentation 
#        and/or other materials provided with the distribution. 
#      o Neither the name of Altera Corporation nor the names of its contributors 
#        may be used to endorse or promote products derived from this software 
#        without specific prior written permission. 
#   
#   THIS SOFTWARE IS PROVIDED BY ALTERA CORPORATION, THE COPYRIGHT HOLDER, AND ITS 
#   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
#   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
#   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
#   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR 
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
#   OF THE POSSIBILITY OF SUCH DAMAGE.  

VPATH = /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/nios2
srcdir = /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/nios2
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
AS = `if [ -f ${objroot}/../gas/as.new ] ; \
	then echo ${objroot}/../gas/as.new ; \
	else echo as ; fi`

AR = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/build-newlib-stage-wrapper nios2-elf-ar

#LD = nios2-elf-ld
LD = `if [ -f ${objroot}/../ld/ld.new ] ; \
	then echo ${objroot}/../ld/ld.new ; \
	else echo ld ; fi`

RANLIB = /data/jcapla9/fingerprinting/automotive_control/rtos/ucos1/newlib-build-tmp/build-newlib-stage-wrapper nios2-elf-ranlib

OBJDUMP = `if [ -f ${objroot}/../binutils/objdump ] ; \
	then echo ${objroot}/../binutils/objdump ; \
	else t='$(program_transform_name)'; echo objdump | sed -e $$t ; fi`
OBJCOPY = `if [ -f ${objroot}/../binutils/objcopy ] ; \
	then echo ${objroot}/../binutils/objcopy ; \
	else t='$(program_transform_name)'; echo objcopy | sed -e $$t ; fi`

CRT0 = crt0.o

# object files common to nosys
COMMONOBJS = \
	nosys_execve.o  \
	nosys_envlock.o  \
	nosys_fork.o \
	nosys_getpid.o  \
	nosys_gettod.o  \
	nosys_kill.o  \
	nosys_link.o  \
	nosys_sbrk.o  \
	nosys_wait.o \
	nosys_mlock.o \
	_zero_bss.o


# Object files specific to particular targets.
NOSYSOBJS = \
	${COMMONOBJS} \
	nosys_close.o  \
	nosys_crt0.o \
	nosys_environ.o  \
	nosys_exit.o  \
	nosys_fstat.o \
	nosys_isatty.o \
	nosys_lseek.o  \
	nosys_open.o  \
	nosys_read.o  \
	nosys_rename.o \
	nosys_stat.o \
	nosys_times.o  \
	nosys_unlink.o \
	nosys_write.o \


STACKOBJS = \
	_stack.o \


GCC_LDFLAGS = `if [ -d ${objroot}/../gcc ] ; \
	then echo -L${objroot}/../gcc ; fi`

OUTPUTS = ${CRT0} libnosys.a libstack.a 

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

libnosys.a: $(NOSYSOBJS)
	${AR} ${ARFLAGS} $@ $(NOSYSOBJS)
	${RANLIB} $@

libstack.a: $(STACKOBJS)
	${AR} ${ARFLAGS} $@ $(STACKOBJS)
	${RANLIB} $@

doc:	

clean mostlyclean:
	rm -f $(OUTPUTS) *.i *~ *.o *-test *.srec *.dis *.map *.x

distclean maintainer-clean realclean: clean
	rm -f Makefile config.status $(OUTPUTS)

.PHONY: install info install-info clean-info
install:
	@for outputs in ${OUTPUTS}; do\
	 mkdir -p $(tooldir)/lib${MULTISUBDIR}; \
	 $(INSTALL_PROGRAM) $${outputs} $(tooldir)/lib${MULTISUBDIR}; \
	done

info:
install-info:
clean-info:

Makefile: Makefile.in config.status /CMC/tools/altera/13.1/nios2eds/bin/gnu/src/newlib-1.16/libgloss/nios2/../config/default.mh
	$(SHELL) config.status

config.status: configure
	$(SHELL) config.status --recheck
