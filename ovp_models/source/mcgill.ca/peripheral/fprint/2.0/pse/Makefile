IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
CFLAGS=-DENDIANBIG
include $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.pse

cleanall: clean
	$(MAKE) -C platform    clean NOVLNV=1
	$(MAKE) -C application clean
	-rm -f *.igen.*
