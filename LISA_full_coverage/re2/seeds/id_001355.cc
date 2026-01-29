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
//<ID> 1355
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>abc)|(def)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabcdefyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char set_pat1[] = "abc";
    const char set_pat2[] = "def";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    int match_idxs[4];
    size_t set_matches = 0;
    const char * pattern_from_re = NULL;
    int err_code = 0;
    int opt_case_flag = 0;
    int opt_log_flag = 0;
    int set_add_ret1 = 0;
    int set_add_ret2 = 0;
    int set_compile_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(match_idxs, 0, sizeof(match_idxs));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    opt_case_flag = cre2_opt_case_sensitive(opt);
    opt_log_flag = cre2_opt_log_errors(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    pattern_from_re = cre2_pattern(re);
    err_code = cre2_error_code(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret1 = cre2_set_add_simple(set, set_pat1);
    set_add_ret2 = cre2_set_add_simple(set, set_pat2);
    set_compile_ret = cre2_set_compile(set);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_idxs, (size_t)4);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)set_matches
                     + (long)match_idxs[0] + (long)match_idxs[1]
                     + (long)err_code + (long)opt_case_flag + (long)opt_log_flag
                     + (long)set_add_ret1 + (long)set_add_ret2 + (long)set_compile_ret
                     + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL);
    (void)computed_check;
    (void)pattern_from_re;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}