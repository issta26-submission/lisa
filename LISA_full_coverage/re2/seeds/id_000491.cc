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
//<ID> 491
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(?: (bar))?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char replace_pat[] = "foo";
    const char text_buf[] = "foo bar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char text_and_target_buf[] = "foo and foo";
    const char rewrite_buf[] = "baz";
    const char extract_text_buf[] = "foo baz";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_text;
    cre2_string_t extract_target;
    int nmatch = 4;
    int match_ret = 0;
    int greplace_ret = 0;
    int extract_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_and_target.data = text_and_target_buf;
    text_and_target.length = (int)(sizeof(text_and_target_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    extract_text.data = extract_text_buf;
    extract_text.length = (int)(sizeof(extract_text_buf) - 1);
    memset(&extract_target, 0, sizeof(extract_target));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    greplace_ret = cre2_global_replace(replace_pat, &text_and_target, &rewrite);
    extract_ret = cre2_extract(replace_pat, &extract_text, &rewrite, &extract_target);

    // step 5: Validate
    computed_check = match_ret + greplace_ret + extract_ret + cre2_num_capturing_groups(re) + match_arr[0].length + text_and_target.length + rewrite.length + extract_text.length + extract_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}