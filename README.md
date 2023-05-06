

Dynamic Memory Allocator: Multi-Pool

• Executed a multi-pool allocator in programming language C and provided memory allocation functions 
such as malloc (), calloc(), and realloc(). Performed free () function to free memory regions allocated by 
any of the above allocation functions



===

Repository Layout
---

The `src/` directory contains the source code for your allocator, as
well as some helper code.  As given, it is a barely-working allocator
that uses `sbrk()` to directly allocate memory from the operating system
for every request.  It does not implement `realloc()`, so many
applications will not run.

You should implement your entire allocator in `src/mm.c`.  The bulk
allocator is also in `src/bulk.c`, but you should not modify the bulk
allocator for your implementation; the Autograder will test your
implementation using the given bulk allocator.

The `tests/` directory contains two simple tests; one of them tests the
bulk allocator (which, as stated above, you should not modify; this test
is just a sanity check to make sure the bulk allocator is working, and
to give you an example of a test), and the other tests to make sure that
`sbrk()` is not called between two requests for the same size
allocation.  Note that this second test _will_ pass with the given
allocator, but that the given allocator is **NOT** a correct allocator;
however, if _your_ allocator has bugs, it may well fail this test!
These two tests should help you see how to design and run your own
tests.

Building the Project
---

The default make target (built by invoking `make`) builds the _shared
object_ (library) file `libcsemalloc.so`.  This object contains your
allocator and can be used to run your standard Unix utilities using your
allocator.  Instructions for how to do this are in the Makefile.


Submission
---

Use `make submission` to build the file `malloc.tar`, which you will
upload to Autograder.

