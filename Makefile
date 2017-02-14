CC=gcc
CFLAGS=-Wall

TEST_FILES=test_runner.c utils.c test_utils.c

test: test_build test_run

test_build:
	@$(CC) $(CFLAGS) $(TEST_FILES) -o test_runner

test_run:
	@-./test_runner

clean:
	rm test_runner
