XCACHE_PROC_SRC=$(srcdir)/processor/main.m4
XCACHE_PROC_OUT=$(builddir)/processor.out
XCACHE_PROC_C=$(builddir)/processor_real.c
XCACHE_PROC_H=$(builddir)/processor.h
XCACHE_INCLUDES_SRC=$(srcdir)/includes.c
XCACHE_INCLUDES_I=$(builddir)/includes.i
XCACHE_STRUCTINFO_OUT=$(builddir)/structinfo.m4

$(XCACHE_INCLUDES_I): $(XCACHE_INCLUDES_SRC) $(srcdir)/xcache.h
	$(CC) -I. -I$(srcdir) $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) -E $(XCACHE_INCLUDES_SRC) -o $(XCACHE_INCLUDES_I)

$(XCACHE_STRUCTINFO_OUT): $(XCACHE_INCLUDES_I) $(srcdir)/mkstructinfo.awk
	@echo $(XCACHE_STRUCTINFO_OUT) is optional if XCache test is not enabled, feel free if it awk failed to produce it
	-$(XCACHE_AWK) -f $(srcdir)/mkstructinfo.awk < $(XCACHE_INCLUDES_I) > $(XCACHE_STRUCTINFO_OUT).tmp && mv $(XCACHE_STRUCTINFO_OUT).tmp $(XCACHE_STRUCTINFO_OUT)

$(XCACHE_PROC_OUT): $(XCACHE_PROC_SRC) $(XCACHE_STRUCTINFO_OUT) $(XCACHE_PROC_SOURCES)
	$(M4) -D srcdir='`'"$(srcdir)'" -D builddir='`'"$(builddir)'" $(XCACHE_ENABLE_TEST) $(XCACHE_PROC_SRC) > $(XCACHE_PROC_OUT).tmp
	mv $(XCACHE_PROC_OUT).tmp $(XCACHE_PROC_OUT)

$(XCACHE_PROC_H): $(XCACHE_PROC_OUT)
	$(GREP) 'export: ' $(XCACHE_PROC_OUT) | $(SED) 's/.*export:\(.*\):export.*/\1/g' > $(XCACHE_PROC_H)
	-$(XCACHE_INDENT) < $(XCACHE_PROC_H) > $(XCACHE_PROC_H).tmp && mv $(XCACHE_PROC_H).tmp $(XCACHE_PROC_H)

$(XCACHE_PROC_C): $(XCACHE_PROC_OUT) $(XCACHE_PROC_H)
	cp $(XCACHE_PROC_OUT) $(XCACHE_PROC_C)
	-$(XCACHE_INDENT) < $(XCACHE_PROC_OUT) > $(XCACHE_PROC_C).tmp && mv $(XCACHE_PROC_C).tmp $(XCACHE_PROC_C)

$(builddir)/processor.lo: $(XCACHE_PROC_C) $(XCACHE_PROC_H) $(srcdir)/processor.c
processor.lo: $(XCACHE_PROC_C) $(XCACHE_PROC_H) $(srcdir)/processor.c

$(builddir)/disassembler.lo: $(XCACHE_PROC_H) $(srcdir)/processor.c
disassembler.lo: $(XCACHE_PROC_H) $(srcdir)/processor.c

$(builddir)/xcache.lo: $(XCACHE_PROC_H) $(srcdir)/xc_shm.h $(srcdir)/stack.h $(srcdir)/xcache_globals.h $(srcdir)/xcache.c $(srcdir)/foreachcoresig.h $(srcdir)/utils.h
xcache.lo: $(XCACHE_PROC_H) $(srcdir)/xc_shm.h $(srcdir)/stack.h $(srcdir)/xcache_globals.h $(srcdir)/xcache.c $(srcdir)/foreachcoresig.h $(srcdir)/utils.h

xcachesvnclean: clean
	cat $(srcdir)/.cvsignore | grep -v ^Makefile | grep -v ^config.nice | xargs rm -rf

xcachetest:
	$(SED) "s#\\./modules/#$(top_builddir)/modules/#" < $(srcdir)/xcache-test.ini > $(top_builddir)/tmp-php.ini
	TEST_PHP_SRCDIR=$(top_srcdir) \
	CC="$(CC)" \
		$(srcdir)/run-xcachetest -d 'open_basedir=' -d 'safe_mode=0' -d 'output_buffering=0' -d 'memory_limit=-1' $(top_srcdir)/run-tests.php $(TESTS) $(TEST_ARGS) -c $(top_builddir)/tmp-php.ini
