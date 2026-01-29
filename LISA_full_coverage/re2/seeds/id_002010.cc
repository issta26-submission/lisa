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
//<ID> 2010
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)[0-9]+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    int log_errors_flag = 0;
    const char text[] = "bar123baz";
    const int text_len = (int)(sizeof(text) - 1);
    int match_buf[8];
    size_t match_count = 0;
    int set_compile_ret = 0;
    long checksum = 0;

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 3: Operate - create a set, add pattern, compile it, and compile a regexp + iterator
    set = cre2_set_new(opt, CRE2_ANCHOR_START);
    cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate - perform a set match and compute a simple checksum
    match_count = cre2_set_match(set, text, text_len, match_buf, 8);
    checksum = (long)pattern_len + (long)log_errors_flag + (long)set_compile_ret + (long)match_count + (long)(pattern_from_re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}