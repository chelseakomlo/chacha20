CC = gcc
CFLAGS = -Wall

TEST_FILES=test_runner.c utils.c test_utils.c

TEST_MAIN = test_runner

test: test_build test_run

test_build:
	@$(CC) $(CFLAGS) $(TEST_FILES) -o $(TEST_MAIN)

test_run:
	@-./$(TEST_MAIN)

clean:
	rm $(TEST_MAIN)

check-leaks:
	valgrind --leak-check=yes --error-limit=no --show-reachable=yes ./$(TEST_MAIN)
