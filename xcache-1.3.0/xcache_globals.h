
ZEND_BEGIN_MODULE_GLOBALS(xcache)
	zend_bool initial_compile_file_called; /* true is origin_compile_file is called */
	zend_bool cacher;      /* true if enabled */
	zend_bool stat;
#ifdef HAVE_XCACHE_OPTIMIZER
	zend_bool optimizer;   /* true if enabled */
#endif
#ifdef HAVE_XCACHE_COVERAGER
	zend_bool coverager;
	zend_bool coverage_enabled;
	HashTable *coverages;  /* coverages[file][line] = times */
#endif
	xc_stack_t *php_holds;
	xc_stack_t *var_holds;
	time_t request_time;
	long   var_ttl;
	zend_bool auth_enabled;

#ifdef ZEND_ENGINE_2
	HashTable gc_op_arrays;
#endif

#ifdef HAVE_XCACHE_CONSTANT
	HashTable internal_constant_table;
#endif
	HashTable internal_function_table;
	HashTable internal_class_table;
	zend_bool internal_table_copied;
ZEND_END_MODULE_GLOBALS(xcache)

ZEND_EXTERN_MODULE_GLOBALS(xcache)

#ifdef ZTS
# define XG(v) TSRMG(xcache_globals_id, zend_xcache_globals *, v)
#else
# define XG(v) (xcache_globals.v)
#endif
