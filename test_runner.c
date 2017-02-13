#include <stdlib.h>
#include <stdio.h>
#include "test_helper.h"

void print_success_message(int total_passed) {
  printf("Total number of tests passed: %d \n", total_passed);
}

void print_failed_message(int total_failed) {
  printf("Total number of tests failed: %d \n", total_failed);
}

void main(void) {
  int total_passed=0;
  int total_failed=0;

  int all_tests_length = sizeof(all_tests)/sizeof(all_tests[0]);

  for (int i=0; i<all_tests_length; i++) {
   int result = all_tests[i](); // execute each test

   result == 1 ? total_passed++ : total_failed++;
  }

  print_success_message(total_passed);
  print_failed_message(total_failed);
}

