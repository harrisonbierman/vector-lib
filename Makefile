CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIB_OBJS = src/vector.o
TEST_OBJS = tests/test_vector.o

# PHONY tells make that these are commands not files
.PHONY: all test clean

all: libvector.a

libvector.a: $(LIB_OBJS)
# $@ is shorthand for all targets. In this case libvector.a
# $^ is shorthand for all prerequisites. In this case $(LIB_OBJS)
	ar rcs $@ $^

test: libvector.a $(TEST_OBJS)
	$(CC) $(CFLAGS) -o run_tests $(TEST_OBJS) -L. -lvector
	./run_tests

clean:
	\rm -f src/*.o tests/*.o libvector.a run_tests

