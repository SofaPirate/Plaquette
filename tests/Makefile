TOPTARGETS := all clean

SUBDIRS := castings filters generators normalizer operations timing

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ EXTRA_CXXFLAGS=-Wno-unused-parameter $(MAKECMDGOALS)

test:
	@for d in $(SUBDIRS); do ./$$d/$$d.out; done

.PHONY: $(TOPTARGETS) $(SUBDIRS)
