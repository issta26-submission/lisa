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
//<ID> 494
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
    const char extract_pattern[] = "(foo) bar";
    const char replace_pattern[] = "foo";
    const char text_buf[] = "foo bar baz foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char replace_buf[] = "foo bar foo";
    const int replace_len = (int)(sizeof(replace_buf) - 1);
    const char rewrite_buf[] = "FOO";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    const char target_buf[] = "";
    const int target_len = (int)(sizeof(target_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t text_str;
    cre2_string_t replace_text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    int nmatch = 3;
    int match_ret = 0;
    int extract_ret = 0;
    int replace_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_str.data = text_buf;
    text_str.length = text_len;
    replace_text_str.data = replace_buf;
    replace_text_str.length = replace_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = target_buf;
    target_str.length = target_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    extract_ret = cre2_extract(extract_pattern, &text_str, &rewrite_str, &target_str);
    replace_ret = cre2_global_replace(replace_pattern, &replace_text_str, &rewrite_str);

    // step 5: Validate
    computed_check = match_ret + extract_ret + replace_ret + match_arr[0].length + text_str.length + replace_text_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}