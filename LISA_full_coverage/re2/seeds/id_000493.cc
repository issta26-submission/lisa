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
//<ID> 493
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern_match[] = "hello (foo)";
    const int pattern_match_len = (int)(sizeof(pattern_match) - 1);
    const char pattern_extract[] = "(foo)";
    const int pattern_extract_len = (int)(sizeof(pattern_extract) - 1);
    const char pattern_replace[] = "foo";
    const int pattern_replace_len = (int)(sizeof(pattern_replace) - 1);
    char text_buf[] = "say hello foo and foo again";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char text_for_replace[] = "one foo two foo";
    const int text_for_replace_len = (int)(sizeof(text_for_replace) - 1);
    char target_buf[64];
    char rewrite_buf[] = "BAR";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_str;
    cre2_string_t replace_text;
    cre2_string_t rewrite_str;
    cre2_string_t extract_target;
    int nmatch = 4;
    int extract_ret = 0;
    int global_ret = 0;
    int match_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(target_buf, 0, sizeof(target_buf));
    text_str.data = text_buf;
    text_str.length = text_len;
    replace_text.data = text_for_replace;
    replace_text.length = text_for_replace_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    extract_target.data = target_buf;
    extract_target.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern_match, pattern_match_len, opt);

    // step 4: Operate
    extract_ret = cre2_extract(pattern_extract, &text_str, &rewrite_str, &extract_target);
    global_ret = cre2_global_replace(pattern_replace, &replace_text, &rewrite_str);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);

    // step 5: Validate
    computed_check = extract_ret + global_ret + match_ret + text_str.length + replace_text.length + rewrite_str.length + match_arr[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}