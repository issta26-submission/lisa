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
//<ID> 2139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>\\w+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_set *set = NULL;
    int add_simple_ret = 0;
    int full_ret = 0;
    int replace_ret = 0;
    int64_t max_mem = 0;
    long checksum = 0;

    // buffers for matching and replacing
    char subject_buf[] = "abc-123";
    cre2_string_t subject = { subject_buf, (int)(sizeof(subject_buf) - 1) };
    char full_target_buf[64];
    memset(full_target_buf, 0, sizeof(full_target_buf));
    cre2_string_t full_target = { full_target_buf, 0 };

    char text_and_target_buf[] = "abc-123 abc-456";
    cre2_string_t text_and_target = { text_and_target_buf, (int)(sizeof(text_and_target_buf) - 1) };
    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);
    max_mem = cre2_opt_max_mem(opt);

    // step 3: Configure (compile regexp, create set and iterator)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_simple_ret = cre2_set_add_simple(set, pattern);

    // step 4: Operate (perform a full match and a replace using compiled regexp)
    full_ret = cre2_full_match(pattern, &subject, &full_target, 10);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)full_ret + (long)replace_ret + (long)add_simple_ret + (long)full_target.length + (long)text_and_target.length + (long)max_mem + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}