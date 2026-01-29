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
//<ID> 495
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
    const char set_pattern[] = "foo|bar";
    const char replace_pattern[] = "foo";
    char mutable_text[] = "xxfoobar foo bar foo";
    const int text_len = (int)(sizeof(mutable_text) - 1);
    char rewrite_buf[128] = "";
    char target_buf[128] = "";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t text_str;
    cre2_string_t target_str;
    int nmatch = 4;
    int add_ret = 0;
    int compile_ret = 0;
    int match_ret = 0;
    int global_ret = 0;
    int extract_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_and_target.data = mutable_text;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 0;
    text_str.data = mutable_text;
    text_str.length = text_len;
    target_str.data = target_buf;
    target_str.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, set_pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Operate
    match_ret = cre2_match(re, mutable_text, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    global_ret = cre2_global_replace(replace_pattern, &text_and_target, &rewrite_str);
    extract_ret = cre2_extract(pattern, &text_str, &rewrite_str, &target_str);

    // step 5: Validate
    computed_check = match_ret + global_ret + extract_ret + add_ret + compile_ret + cre2_num_capturing_groups(re) + match_arr[0].length + text_and_target.length + rewrite_str.length + target_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}