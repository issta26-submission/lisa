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
//<ID> 1394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "abc";
    const size_t set_pattern_len = (size_t)(sizeof(set_pattern) - 1);
    const char replace_pattern[] = "o";
    const char text_buf[] = "foobarfoo";
    const char rewrite_buf[] = "X";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char errbuf[128];
    long computed_check = 0;
    int added_ret = 0;
    int num_groups = 0;
    int rep_ret1 = 0;
    int rep_ret2 = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    memset(errbuf, 0, sizeof(errbuf));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    added_ret = cre2_set_add(set, set_pattern, set_pattern_len, errbuf, sizeof(errbuf));
    num_groups = cre2_num_capturing_groups(re);
    rep_ret1 = cre2_replace(replace_pattern, &text_and_target, &rewrite);
    rep_ret2 = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)num_groups + (long)added_ret
                     + (long)rep_ret1 + (long)rep_ret2
                     + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL)
                     + (long)text_and_target.length + (long)rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}