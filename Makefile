CC=gcc

test: test_build test_run

test_build:
	@$(CC) test_runner.c -o test_runner

test_run:
	@-./test_runner

clean:
	rm test_runner
