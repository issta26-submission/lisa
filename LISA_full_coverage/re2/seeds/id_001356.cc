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
//<ID> 1356
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>abc)|(def)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "zzabcdefyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    const char * pattern_from_re = NULL;
    int cs_flag = 0;
    int log_err_flag = 0;
    int add_ret = 0;
    int compile_ret = 0;
    int match_indices[4];
    size_t match_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(match_indices, 0, sizeof(match_indices));

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cs_flag = cre2_opt_case_sensitive(opt);
    log_err_flag = cre2_opt_log_errors(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_ANCHOR_BOTH);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)cs_flag + (long)log_err_flag
                     + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL)
                     + (long)match_count + (long)add_ret + (long)compile_ret + (long)match_indices[0]
                     + (long)(pattern_from_re != NULL);
    (void)computed_check;
    (void)pattern_from_re;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}