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
//<ID> 1152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_for_match[] = "prefix foo123bar suffix";
    const int text_for_match_len = (int)(sizeof(text_for_match) - 1);
    char text_for_replace[] = "foo123bar and foo456bar and foo789bar";
    const int text_for_replace_len = (int)(sizeof(text_for_replace) - 1);
    char rewrite_buf[] = "X";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int match_ret = 0;
    int replace_ret = 0;
    int errcode = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "foo[0-9]+bar");

    // step 3: Operate
    memset(matches, 0, sizeof(matches));
    match_ret = cre2_match(re, text_for_match, text_for_match_len, 0, text_for_match_len, CRE2_UNANCHORED, matches, nmatch);
    text_and_target.data = text_for_replace;
    text_and_target.length = text_for_replace_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    errcode = cre2_error_code(re);
    computed_check = (long)match_ret + (long)replace_ret + (long)errcode + (long)matches[0].length + (long)text_and_target.length + (long)rewrite.length + (long)pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}