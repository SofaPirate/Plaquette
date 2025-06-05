TOPTARGETS := all clean

SUBDIRS := arrays castings engines filters functions generators hybridarrays normalizer operations timing

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ EXTRA_CXXFLAGS="-Wno-unused-parameter" $(MAKECMDGOALS)

test:
	@for d in $(SUBDIRS); do ./$$d/$$d.out; done

.PHONY: $(TOPTARGETS) $(SUBDIRS)
