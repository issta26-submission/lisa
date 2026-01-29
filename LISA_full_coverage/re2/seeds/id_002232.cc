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
//<ID> 2232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxx foo yyy fooz foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    int match_indices[8];
    size_t match_count = 0;
    int literal_flag = 0;
    int64_t max_mem = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    max_mem = cre2_opt_max_mem(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)(sizeof(match_indices) / sizeof(match_indices[0])));

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)match_count + (long)match_indices[0]
             + (long)literal_flag + (long)(max_mem & 0x7fffffff) + (long)set_add_ret + (long)set_compile_ret;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}