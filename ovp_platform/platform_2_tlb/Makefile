#This one includes the intercept stuff

IMPERAS_HOME := $(shell getpath.exe "$(IMPERAS_HOME)")
include $(IMPERAS_HOME)/bin/Makefile.include

# Run the simulator using the imperas front end console
SIMULATOR?=imperas.exe

all: platform intercept 

platform::
	$(MAKE) -C FprintPlatform/1.0/ NOVLNV=1

intercept::
	$(MAKE) -f Makefile.intercept NOVLNV=1

application::
	$(MAKE) -C $@ 

run1:
	$(SIMULATOR) \
      --icmobject FprintPlatform/1.0/platform.$(IMPERAS_ARCH).$(IMPERAS_SHRSUF) \
      -o simulator.log

run2: 
	$(SIMULATOR) \
      --icmobject FprintPlatform/1.0/platform.$(IMPERAS_ARCH).$(IMPERAS_SHRSUF) \
      --extlib model.$(IMPERAS_SHRSUF) \
      -o simulator.log 

debug:
	$(SIMULATOR) \
      --icmobject FprintPlatform/1.0/platform.$(IMPERAS_ARCH).$(IMPERAS_SHRSUF) \
      --extlib model.$(IMPERAS_SHRSUF) --idebug\
      -o simulator.log   

debug1:
	$(SIMULATOR) \
      --icmobject FprintPlatform/1.0/platform.$(IMPERAS_ARCH).$(IMPERAS_SHRSUF) \
      -o simulator.log    --idebug



ic-run1:
	IMPERAS_RUNTIME=CpuManager FprintPlatform/1.0/platform.$(IMPERAS_ARCH).exe application/application.OR1K.elf 

ic-run2: loadExtLib.ic
	IMPERAS_RUNTIME=CpuManager IMPERAS_TOOLS=loadExtLib.ic pFprintPlatform/1.0/platform.$(IMPERAS_ARCH).exe application/application.OR1K.elf

loadExtLib.ic:
	$(V) echo "--extlib CPU1=modelAttrs@model.$(IMPERAS_SHRSUF)" > loadExtLib.ic

clean:
	$(MAKE) -C FprintPlatform/1.0/ NOVLNV=1 clean
	$(MAKE) -f Makefile.intercept NOVLNV=1 clean
	@rm -rf obj/


help:
	@echo The following targets are available to start simulation
	@echo run1    : run simulation using $(SIMULATOR), no interception library
	@echo run2    : run simulation using $(SIMULATOR), interception library to detect issues
	@echo ic-run1 : run simulation using ICM Platform, no interception library
	@echo ic-run2 : run simulation using ICM Platform, interception library to detect issues
