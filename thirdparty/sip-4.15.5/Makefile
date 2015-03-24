
all:
	@(cd sipgen; $(MAKE))
	@(cd siplib; $(MAKE))

install:
	@(cd sipgen; $(MAKE) install)
	@(cd siplib; $(MAKE) install)
	@test -d $(DESTDIR)/home/jules/SAMOGWAS/Tulip4.6/build-debug/thirdparty/sip-4.15.5/siplib || mkdir -p $(DESTDIR)/home/jules/SAMOGWAS/Tulip4.6/build-debug/thirdparty/sip-4.15.5/siplib
	cp -f sipconfig.py $(DESTDIR)/home/jules/SAMOGWAS/Tulip4.6/build-debug/thirdparty/sip-4.15.5/siplib/sipconfig.py
	cp -f /home/jules/SAMOGWAS/Tulip4.6/tulip-src/thirdparty/sip-4.15.5/sipdistutils.py $(DESTDIR)/home/jules/SAMOGWAS/Tulip4.6/build-debug/thirdparty/sip-4.15.5/siplib/sipdistutils.py

clean:
	@(cd sipgen; $(MAKE) clean)
	@(cd siplib; $(MAKE) clean)
