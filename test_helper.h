#include "chacha20_test.c"
#include "utils_test.c"

#ifndef TEST_HELPER_H
#define TEST_HELPER_H

int (*all_tests[])(void) = {
  test_build_block,
  test_apply_qround,
  test_apply_rules,
  test_to_uint32,
  test_to_uint32_high_val,
  test_block_function,
  test_block_function_second_case,
  test_block_function_third_case,
  test_serialize,
  test_block_function_api,
  test_slice_starting_position,
  test_slice_ending_position,
  test_build_block_and_serialize,
  test_chacha20_encrypt
};

#endif
