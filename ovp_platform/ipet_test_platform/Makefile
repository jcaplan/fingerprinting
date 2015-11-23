IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")

# Build locally without using a VLNV library
NOVLNV=1

# Build using two passes so that each pass includes separate Makefiles
MAKEPASS?=0
ifeq ($(MAKEPASS),0)
all: application platform
platform:
	$(MAKE) MAKEPASS=1
application:
	$(MAKE) MAKEPASS=2
clean:
	$(MAKE) MAKEPASS=1 clean
	$(MAKE) MAKEPASS=2 clean
endif

# Pass 1 build the Platform
ifeq ($(MAKEPASS),1)

SRC=OVPsim_Nios_II.c
include $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.platform
endif

# Pass 2 build the Application
ifeq ($(MAKEPASS),2)

OPTIMISATION?=-O3 -g -g -gdwarf-2
PKSPD_OPTIMISATION?=-O0 -g -gdwarf-2

# provide alternate build controls
MULT?=
DEF?=

CROSS=CS_NIOS2
SRC?=linpack.c fibonacci.c dhrystone.c peakSpeed2.c
SRCINC?=

INCOBJ=$(patsubst %.c,%$(MULT).$(CROSS).o,$(SRCINC))
EXE=$(patsubst %.c,%$(MULT).$(CROSS).elf,$(SRC))


-include $(IMPERAS_HOME)/lib/$(IMPERAS_ARCH)/CrossCompiler/$(CROSS).makefile.include
ifeq ($($(CROSS)_CC),)
    IMPERAS_ERROR := $(error "Error : $(CROSS)_CC not set. Please check installation of toolchain for $(CROSS)")
endif

all: $(EXE)

%$(MULT).$(CROSS).elf: %$(MULT).$(CROSS).o $(INCOBJ)
	@    echo "# Linking $(CROSS) $@"
	$(V) $(IMPERAS_LINK) -o $@ $^ $(IMPERAS_LDFLAGS) -lm

peakSpeed2$(MULT).$(CROSS).o: peakSpeed2.c
	@    echo "# Compiling $(CROSS) $<"
	$(V) $(IMPERAS_CC) -c -o $@ $< -D$(CROSS) $(DEF) $(PKSPD_OPTIMISATION)

%$(MULT).$(CROSS).o: %.c
	@    echo "# Compiling $(CROSS) $<"
	$(V) $(IMPERAS_CC) -c -o $@ $< -D$(CROSS) $(DEF) $(OPTIMISATION)

%$(MULT).dbg.$(CROSS).elf: %$(MULT).dbg.$(CROSS).o $(INCOBJ)
	@    echo "# Linking $(CROSS) $@"
	$(V) $(IMPERAS_LINK) -o $@ $^ $(IMPERAS_LDFLAGS) -lm

%$(MULT).dbg.$(CROSS).o: %.c
	@    echo "# Compiling $(CROSS) (Debug) $<"
	$(V) $(IMPERAS_CC) -c -o $@ $< -D$(CROSS) $(DEF) -O0 -g -gdwarf-2 

clean::
	- rm -f *.$(CROSS).elf *.$(CROSS).o

endif

