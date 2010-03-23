SUBDIRS = ../../

all-am:
	list='	$(SUBDIRS)'; for subdir in $$list; do \
	test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS)); \
	done