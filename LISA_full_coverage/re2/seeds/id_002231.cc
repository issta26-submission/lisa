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
//<ID> 2231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "foo|bar|baz";
    const int pattern1_len = (int)(sizeof(pattern1) - 1);
    char text_buf[] = "this is foo and that is bar";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    int match[8];
    size_t match_len = sizeof(match) / sizeof(match[0]);
    int add_ret = 0;
    int compile_ret = 0;
    size_t nmatch = 0;
    int literal_flag = 0;
    int64_t max_mem_val = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)4 * 1024 * 1024);

    // step 3: Core operations
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret = cre2_set_add_simple(set, pattern1);
    compile_ret = cre2_set_compile(set);
    nmatch = cre2_set_match(set, text_buf, text_len, match, match_len);

    // step 4: Validate
    literal_flag = cre2_opt_literal(opt);
    max_mem_val = cre2_opt_max_mem(opt);
    checksum = (long)pattern1_len + (long)text_len + (long)add_ret + (long)compile_ret + (long)nmatch
             + (long)match[0] + (long)match[1] + (long)literal_flag + (long)max_mem_val + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}