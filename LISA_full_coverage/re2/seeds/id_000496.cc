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
//<ID> 496
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
    const char replace_pattern[] = "foo";
    const char initial_text[] = "foo bar foo baz";
    const int text_len = (int)(sizeof(initial_text) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t text_str;
    cre2_string_t target;
    int nmatch = 4;
    int match_ret = 0;
    int replace_ret = 0;
    int extract_ret = 0;
    int group_count = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    char mutable_text[128];
    memset(mutable_text, 0, sizeof(mutable_text));
    memcpy(mutable_text, initial_text, (size_t)text_len);
    text_and_target.data = mutable_text;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;
    text_str.data = mutable_text;
    text_str.length = text_len;
    char target_buf[64];
    memset(target_buf, 0, sizeof(target_buf));
    target.data = target_buf;
    target.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    match_ret = cre2_match(re, mutable_text, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    replace_ret = cre2_global_replace(replace_pattern, &text_and_target, &rewrite);
    extract_ret = cre2_extract(pattern, &text_str, &rewrite, &target);

    // step 5: Validate
    group_count = cre2_num_capturing_groups(re);
    computed_check = match_ret + replace_ret + extract_ret + group_count + match_arr[0].length + target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}