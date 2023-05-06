CC := gcc

# You may change this if you like.  However, the grader will use these
# CFLAGS!  Note that _DEFAULT_SOURCE must be defined for your source to
# find the functions brk() and sbrk(), and -fPIC is required for
# building the shared library libcsemalloc.so.
CFLAGS := -g -Wall -Werror -std=c99 -fPIC -D_DEFAULT_SOURCE

# These are the included tests.  You may modify this line if you like,
# but your modifications will not be submitted.  (You might, for
# example, want to temporarily remove tests that are known to fail.)
#
# You can add tests to this list that will be compiled and run when you
# invoke make test.  See the test and tests/% rules, below.
TESTS := test_bulk test_malloc_free test_more_malloc_free test_different_malloc_free test_relloc_free

all: libcsemalloc.so

# This rule generates an ELF shared object that can be used to test your
# malloc against any UNIX application!  Applications that use threading
# are likely to fail in unexpected ways, but single-threaded
# applications will probably work fine.
#
# You can use this library by running the application as follows:
#
# LD_PRELOAD=./libcsemalloc.so command
#
# For example, to use your malloc implementation in the standard UNIX ls
# command, you would run:
#
# LD_PRELOAD=./libcsemalloc.so ls
#
# Note that the given code will not successfully run ls, as it does not
# implement realloc.  It will, however, run `ls --help` and several
# other commands (that do not use realloc).
libcsemalloc.so: src/mm.o src/bulk.o
	$(CC) -shared -fPIC -o $@ $^

test: $(TESTS) $(NEWTESTS)
	@echo
	@for test in $^; do                                   \
	    printf "Running %-30s: " "$$test";                \
	    (./$$test && echo "passed") || echo "failed";       \
	done
	@echo

# This rule ensures that 'make submission' builds the tar file that you
# must submit to Autograder.
submission: malloc.tar

# This rule creates the submission tarfile.  You should not change it.
malloc.tar: src/mm.c
	tar cf $@ $^

# This pattern rule should be starting to look familiar.  It will use
# the definitions of CC and CFLAGS, above, to create an object file from
# a source C file.
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# This pattern will build any self-contained test file in tests/.  If
# your test file needs more support, you will need to write an explicit
# rule for it.
#
# To add a test, create a file called tests/testname.c that contains a
# main function and all of the relevant test code, then add the basename
# of the file (e.g., testname in this example) to TESTS, above.
%: tests/%.o src/mm.o src/bulk.o
	$(CC) -o $@ $^

clean:
	rm -f $(TESTS) libcsemalloc.so malloc.tar
	rm -f src/*.o tests/*.o *~ src/*~ tests/*~

# See previous assignments for a description of .PHONY
.PHONY: all clean submission test
