# make && sudo make install && make testprog && make runtest
include Makefile.inc

threaded ?= 1
export threaded

TARGET = $(LIBTARGET)
TARGET_FULL = $(TARGET).$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)
LIBHEADERS = dwtct.h dwtctconf.h


.PHONE: all install clean cleanall uninstall testprog runtest

all: 
	@make -C $(SRCDIR)

install:
	$(INSTALL) $(SRCDIR)/$(TARGET) $(LIBDIR)/$(TARGET_FULL)
	$(LN) $(LIBDIR)/$(TARGET_FULL) $(LIBDIR)/$(TARGET).$(VERSION_MAJOR).$(VERSION_MINOR)
	$(LN) $(LIBDIR)/$(TARGET_FULL) $(LIBDIR)/$(TARGET).$(VERSION_MAJOR)
	$(LN) $(LIBDIR)/$(TARGET_FULL) $(LIBDIR)/$(TARGET)
	echo $(LIBDIR) > /etc/ld.so.conf.d/$(NAME).conf && ldconfig
	for file in $(LIBHEADERS) ; do \
		$(INSTALL) -m 644 $(SRCDIR)/$$file $(INCDIR) ; \
	done

clean:
	@make -C $(SRCDIR) clean

cleanall: clean
	@make -C $(TESTDIR) clean

uninstall:
	$(RM) $(LIBDIR)/$(TARGET_FULL)
	$(RM) $(LIBDIR)/$(TARGET).$(VERSION_MAJOR).$(VERSION_MINOR)
	$(RM) $(LIBDIR)/$(TARGET).$(VERSION_MAJOR)
	$(RM) $(LIBDIR)/$(TARGET)
	$(RM) /etc/ld.so.conf.d/$(NAME).conf
	for file in $(LIBHEADERS) ; do \
		$(RM) $(INCDIR)/$$file ; \
	done

testprog: 
	@make -C $(TESTDIR)

runtest: 
	@make --quiet -C $(TESTDIR) run
