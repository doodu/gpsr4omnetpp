#
#  Makefile for 802.11
#
#  ** This file was automatically generated by the command:
#  opp_makemake -f -b /root/graduation/src/mobility-fw2.0p3 -c /root/graduation/src/mobility-fw2.0p3/omnetppconfig -I/root/graduation/src/mobility-fw2.0p3/core/include -I/root/graduation/src/mobility-fw2.0p3/contrib/include -L/root/graduation/src/mobility-fw2.0p3/core/lib -lmfcore -L/root/graduation/src/mobility-fw2.0p3/contrib/lib -lmfcontrib
#

# Name of target to be created (-o option)
TARGET = 802.11

# User interface (uncomment one) (-u option)
# USERIF_LIBS=$(CMDENV_LIBS)
USERIF_LIBS=$(TKENV_LIBS)

# .ned or .h include paths with -I
INCLUDE_PATH= -I../../core/include -I../../contrib/include

# misc additional object and library files to link
EXTRA_OBJS=

# object files from other directories to link with (wildcard needed to prevent "no such file *.o" errors)
EXT_DIR_OBJS=

# time stamps of other directories (used as dependency)
EXT_DIR_TSTAMPS= ../../core/include/.tstamp ../../contrib/include/.tstamp

# Additional libraries (-L option -l option)
LIBS= -L../../core/lib -L../../contrib/lib -lmfcore -lmfcontrib

#------------------------------------------------------------------------------
# Import generic settings from ../../omnetppconfig
include ../../omnetppconfig

MSGCOPTS= $(INCLUDE_PATH)

#------------------------------------------------------------------------------

# subdirectories to recurse into
SUBDIRS= 

# object files in this directory
OBJS= Host_n.o Network_n.o  BlueqApplLayer_n.o BlueqApplLayer.o\
	 GPSRNetwLayer.o GPSRNetwLayer_n.o GPSRPkt_m.o

# header files generated (from msg files)
GENERATEDHEADERS= 

#------------------------------------------------------------------------------

$(TARGET): $(OBJS) $(EXTRA_OBJS) $(EXT_DIR_TSTAMPS) Makefile
	$(CXX) $(LDFLAGS) $(OBJS) $(EXTRA_OBJS) $(EXT_DIR_OBJS) $(LIBS) $(OMNETPP_LIBS) -o $(TARGET)
	echo>.tstamp

$(OBJS) : $(GENERATEDHEADERS)


purify: $(OBJS) $(EXTRA_OBJS) $(EXT_DIR_TSTAMPS) Makefile
	purify $(CXX) $(LDFLAGS) $(OBJS) $(EXTRA_OBJS) $(EXT_DIR_OBJS) $(LIBS) -L$(OMNETPP_LIB_DIR) $(KERNEL_LIBS) $(USERIF_LIBS) $(SYS_LIBS_PURE) -o $(TARGET).pure

$(EXT_DIR_TSTAMPS):
	@echo
	@echo Error: $@ does not exist.
	@echo This means that at least the above dependency directory has not been built.
	@echo Maybe you need to do a top-level make?
	@echo
	@exit 1

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

Host_n.o: Host_n.cc
	$(CXX) -c $(NEDCOPTS) Host_n.cc
Host_n.cc: Host.ned
	$(NEDC) $(INCLUDE_PATH) Host.ned

Network_n.o: Network_n.cc
	$(CXX) -c $(NEDCOPTS) Network_n.cc
Network_n.cc: Network.ned
	$(NEDC) $(INCLUDE_PATH) Network.ned

BlueqApplLayer_n.cc: BlueqApplLayer.ned
	$(NEDC) $(INCLUDE_PATH) $<
BlueqApplLayer_n.o: BlueqApplLayer_n.cc
	$(CXX) -c $(NEDCOPTS) $<
BlueqApplLayer.o: BlueqApplLayer.cc
	$(CXX) -c $(COPTS) $<

GPSRNetwLayer_n.cc:GPSRNetwLayer.ned
	$(NEDC) $(INCLUDE_PATH) $<
GPSRNetwLayer_n.o: GPSRNetwLayer_n.cc
	$(CXX) -c $(NEDCOPTS) $<
GPSRNetwLayer.o: GPSRNetwLayer.cc
	$(CXX) -c $(COPTS) $<


GPSRPkt_m.o: GPSRPkt_m.cc
	$(CXX) -c $(NEDCOPTS) $<
GPSRPkt_m.cc GPSRPkt_m.h: GPSRPkt.msg
	$(MSGC) $(MSGCOPTS) $<

GPSRNetwLayer.o: GPSRPkt_m.h
GPSRPkt_m.o: GPSRPkt.msg

#doc: neddoc doxy

#neddoc:
#	opp_neddoc -a

#doxy: doxy.cfg
#	doxygen doxy.cfg

generateheaders: $(GENERATEDHEADERS)
	for i in $(SUBDIRS); do (cd $$i && $(MAKE) generateheaders) || exit 1; done

clean:
	rm -f $(TARGET)$(EXE_SUFFIX)
	rm -f *.o *_n.cc *_n.h *_m.cc *_m.h .tstamp *~
	rm -f *.vec *.sca
	for i in $(SUBDIRS); do (cd $$i && $(MAKE) clean); done

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -- *.cc
	# $(MAKEDEPEND) $(INCLUDE_PATH) -fMakefile.in -- *.cc
	for i in $(SUBDIRS); do (cd $$i && $(MAKE) depend) || exit 1; done

makefiles:
	# recreate Makefile
	opp_makemake -f  -b /root/graduation/src/mobility-fw2.0p3 -c /root/graduation/src/mobility-fw2.0p3/omnetppconfig -I/root/graduation/src/mobility-fw2.0p3/core/include -I/root/graduation/src/mobility-fw2.0p3/contrib/include -L/root/graduation/src/mobility-fw2.0p3/core/lib -lmfcore -L/root/graduation/src/mobility-fw2.0p3/contrib/lib -lmfcontrib 
	for i in $(SUBDIRS); do (cd $$i && $(MAKE) makefiles) || exit 1; done

makefile-ins:
	# recreate Makefile.in
	opp_makemake -f -m  -b /root/graduation/src/mobility-fw2.0p3 -c /root/graduation/src/mobility-fw2.0p3/omnetppconfig -I/root/graduation/src/mobility-fw2.0p3/core/include -I/root/graduation/src/mobility-fw2.0p3/contrib/include -L/root/graduation/src/mobility-fw2.0p3/core/lib -lmfcore -L/root/graduation/src/mobility-fw2.0p3/contrib/lib -lmfcontrib 
	for i in $(SUBDIRS); do (cd $$i && $(MAKE) makefile-ins) || exit 1; done

# "re-makemake" and "re-makemake-m" are deprecated, historic names of the above two targets
re-makemake: makefiles
re-makemake-m: makefile-ins


# DO NOT DELETE THIS LINE -- make depend depends on it.
Host_n.o: Host_n.cc
Network_n.o: Network_n.cc 
