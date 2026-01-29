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
//<ID> 2233
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
    char text_buf[] = "this text contains foo and also baz somewhere";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int match_buf[16];
    size_t match_len = (size_t)(sizeof(match_buf) / sizeof(match_buf[0]));
    size_t found = 0;
    int literal_flag = 0;
    long long max_mem_val = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match_buf, 0, sizeof(match_buf));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate
    found = cre2_set_match(set, text_buf, (size_t)text_len, match_buf, match_len);

    // step 5: Validate
    literal_flag = cre2_opt_literal(opt);
    max_mem_val = (long long)cre2_opt_max_mem(opt);
    checksum = (long)pattern_len + (long)found + (long)match_buf[0] + (long)set_add_ret + (long)set_compile_ret
             + (long)literal_flag + (long)max_mem_val + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}