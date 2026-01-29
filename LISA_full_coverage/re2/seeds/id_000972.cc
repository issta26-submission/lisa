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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<word>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123 bob456 carol789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite1_buf[128];
    char rewrite2_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite1;
    cre2_string_t rewrite2;
    cre2_string_t match;
    int replace_res1 = 0;
    int replace_res2 = 0;
    int match_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate - prepare buffers and do a global replace using pattern string API
    memset(rewrite1_buf, 0, sizeof(rewrite1_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite1.data = rewrite1_buf;
    rewrite1.length = 0;
    replace_res1 = cre2_global_replace(pattern, &text_and_target, &rewrite1);

    // step 4: Operate - compile a regexp, create named group iterator, and do a global replace using compiled re
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    memset(rewrite2_buf, 0, sizeof(rewrite2_buf));
    rewrite2.data = rewrite2_buf;
    rewrite2.length = 0;
    replace_res2 = cre2_global_replace_re(re, &text_and_target, &rewrite2);

    // step 5: Operate - try an easy match to extract first match into match buffer
    memset(match_buf, 0, sizeof(match_buf));
    match.data = match_buf;
    match.length = 0;
    match_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 6: Validate / Cleanup
    computed_check = (long)replace_res1 + (long)replace_res2 + (long)match_res + (long)rewrite1.length + (long)rewrite2.length + (long)match.length;
    (void)computed_check;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}