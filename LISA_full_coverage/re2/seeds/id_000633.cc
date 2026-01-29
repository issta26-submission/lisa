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
//<ID> 633
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char initial_text[] = "abc abc";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_buf[] = "Z";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    int match_arr[8];
    size_t matched_count = 0;
    int rc_replace = 0;
    int rc_add = 0;
    int rc_compile = 0;
    int rev = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_and_target.data = initial_text;
    text_and_target.length = initial_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    rev = cre2_version_interface_revision();
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 4: Operate
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite_str);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    matched_count = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, match_arr, (size_t)(sizeof(match_arr)/sizeof(match_arr[0])));

    // step 5: Validate
    computed_check = rev + rc_replace + rc_add + rc_compile + (int)matched_count + text_and_target.length + rewrite_str.length + pattern_len + initial_text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}