TOPTARGETS := all clean

SUBDIRS := arrays castings engines filters functions generators hybridarrays normalizer operations timing values

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ EXTRA_CXXFLAGS="-Wno-unused-parameter -DPQ_OPTIMIZE_FOR_CPU=1" $(MAKECMDGOALS)

test:
	@for d in $(SUBDIRS); do ./$$d/$$d.out; done

.PHONY: $(TOPTARGETS) $(SUBDIRS)
