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
//<ID> 2236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "Alice";
    const char pattern2[] = "foo";
    char text_buf[] = "Alice foo bar Alice baz";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    int match_array[8];
    size_t match_count = 0;
    int set_add_ret1 = 0;
    int set_add_ret2 = 0;
    int set_compile_ret = 0;
    int64_t max_mem_val = 0;
    int literal_flag = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match_array, 0, sizeof(match_array));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(16 * 1024 * 1024));

    // step 3: Inspect option values
    max_mem_val = cre2_opt_max_mem(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 4: Build and compile a set of patterns
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret1 = cre2_set_add_simple(set, pattern1);
    set_add_ret2 = cre2_set_add_simple(set, pattern2);
    set_compile_ret = cre2_set_compile(set);

    // step 5: Operate (match the text against the compiled set)
    match_count = cre2_set_match(set, text_buf, text_len, match_array, (size_t)(sizeof(match_array) / sizeof(match_array[0])));

    // step 6: Validate and cleanup
    checksum = (long)text_len + (long)max_mem_val + (long)literal_flag + (long)match_count
             + (long)set_add_ret1 + (long)set_add_ret2 + (long)set_compile_ret + (long)(match_array[0])
             + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}