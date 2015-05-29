VLNVSRC  = $(shell pwd)/source
VLNVROOT = $(shell pwd)/lib

all:
	$(MAKE) -C $(VLNVSRC) VLNVSRC=$(VLNVSRC) VLNVROOT=$(VLNVROOT) -f $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.library VERBOSE=0

clean:
	$(MAKE) -C $(VLNVSRC) VLNVSRC=$(VLNVSRC) VLNVROOT=$(VLNVROOT) -f $(IMPERAS_HOME)/ImperasLib/buildutils/Makefile.library VERBOSE=0 clean

