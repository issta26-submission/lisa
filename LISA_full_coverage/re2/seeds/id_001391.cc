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
//<ID> 1391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char initial_text[] = "alpha foo beta foo gamma";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_text[] = "REPLACED";
    const int rewrite_text_len = (int)(sizeof(rewrite_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char add_error[128];
    int set_add_ret = 0;
    int replace_ret = 0;
    int greplace_ret = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    memset(add_error, 0, sizeof(add_error));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Core operations
    set_add_ret = cre2_set_add(set, pattern, (size_t)pattern_len, add_error, sizeof(add_error));
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    text_and_target.data = initial_text;
    text_and_target.length = initial_text_len;
    rewrite.data = rewrite_text;
    rewrite.length = rewrite_text_len;
    replace_ret = cre2_replace("foo", &text_and_target, &rewrite);
    greplace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)initial_text_len + (long)rewrite_text_len
                     + (long)set_add_ret + (long)replace_ret + (long)greplace_ret
                     + (long)num_groups + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;
    (void)add_error;
    (void)text_and_target;
    (void)rewrite;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}