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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char re_pattern[] = "(foo)(?:\\s+bar)?";
    const int re_pattern_len = (int)(sizeof(re_pattern) - 1);
    const char replace_pattern[] = "foo";
    const char extract_pattern[] = "(foo)";
    const char text_buf[] = "foo bar baz foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "X";
    char extract_rewrite_buf[] = "$1";
    char extract_target_buf[64];
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_rewrite;
    cre2_string_t extract_target;
    int nmatch = 3;
    int match_ret = 0;
    int replace_ret = 0;
    int extract_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    extract_rewrite.data = extract_rewrite_buf;
    extract_rewrite.length = (int)(sizeof(extract_rewrite_buf) - 1);
    extract_target.data = extract_target_buf;
    extract_target.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Operate
    re = cre2_new(re_pattern, re_pattern_len, opt);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    replace_ret = cre2_global_replace(replace_pattern, &text_and_target, &rewrite);
    extract_ret = cre2_extract(extract_pattern, &text_and_target, &extract_rewrite, &extract_target);

    // step 4: Validate
    computed_check = match_ret + replace_ret + extract_ret + match_arr[0].length + text_and_target.length + extract_target.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}