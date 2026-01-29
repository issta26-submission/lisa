#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 2239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo|bar|baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "here is foo and baz and qux";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    int match_arr[4];
    size_t match_len = (size_t)(sizeof(match_arr) / sizeof(match_arr[0]));
    size_t found = 0;
    int is_literal = 0;
    int64_t max_mem = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    long checksum = 0;

    // step 2: Setup (initialize options and configure)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    is_literal = cre2_opt_literal(opt);
    max_mem = cre2_opt_max_mem(opt);

    // step 3: Core operations (build and compile a set of patterns)
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate (perform a match against the compiled set)
    memset(match_arr, 0, sizeof(match_arr));
    found = cre2_set_match(set, text_buf, text_len, match_arr, match_len);

    // step 5: Validate (combine results into a checksum to tie outputs)
    checksum = (long)pattern_len + (long)text_len + (long)found + (long)match_arr[0] + (long)is_literal + (long)(max_mem & 0x7fffffff)
             + (long)set_add_ret + (long)set_compile_ret + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}