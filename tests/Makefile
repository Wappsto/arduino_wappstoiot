tests:
	set -e; \
	for i in *Test/Makefile; do \
		echo '==== Making:' $$(dirname $$i); \
		#$(MAKE) -C $$(dirname $$i) -j EXTRA_CXXFLAGS+=-DARDUINOJSON_ENABLE_ARDUINO_STRING EXTRA_CXXFLAGS+=-fprofile-arcs EXTRA_CXXFLAGS+=-ftest-coverage EXTRA_CXXFLAGS+=-fPIC LDFLAGS+=-lgcov; \
		$(MAKE) -C $$(dirname $$i) -j EXTRA_CXXFLAGS+=-DARDUINOJSON_ENABLE_ARDUINO_STRING; \
	done

runtests:
	set -e; \
	for i in *Test/Makefile; do \
		echo '==== Running:' $$(dirname $$i); \
		$$(dirname $$i)/$$(dirname $$i).out; \
	done

clean:
	set -e; \
	for i in *Test/Makefile; do \
		echo '==== Cleaning:' $$(dirname $$i); \
		$(MAKE) -C $$(dirname $$i) clean; \
	done
