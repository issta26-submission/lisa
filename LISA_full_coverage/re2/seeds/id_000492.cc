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
//<ID> 492
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
    const char text_buf[] = "xxfoobar foo bar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char extract_pattern[] = "(foo) (bar)";
    const char extract_text_buf[] = "foo bar";
    const char extract_rewrite_buf[] = "$1-$2";
    char extract_target_buf[64];
    const char replace_pattern[] = "foo";
    char replace_text_buf[] = "foo foo baz";
    const char replace_rewrite_buf[] = "X";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_str;
    cre2_string_t extract_text;
    cre2_string_t extract_rewrite;
    cre2_string_t extract_target;
    cre2_string_t replace_text_and_target;
    cre2_string_t replace_rewrite;
    int nmatch = 4;
    int match_ret = 0;
    int extract_ret = 0;
    int replace_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_str.data = text_buf;
    text_str.length = text_len;
    extract_text.data = extract_text_buf;
    extract_text.length = (int)(sizeof(extract_text_buf) - 1);
    extract_rewrite.data = extract_rewrite_buf;
    extract_rewrite.length = (int)(sizeof(extract_rewrite_buf) - 1);
    extract_target.data = extract_target_buf;
    extract_target.length = 0;
    replace_text_and_target.data = replace_text_buf;
    replace_text_and_target.length = (int)(sizeof(replace_text_buf) - 1);
    replace_rewrite.data = replace_rewrite_buf;
    replace_rewrite.length = (int)(sizeof(replace_rewrite_buf) - 1);
    memset(extract_target_buf, 0, sizeof(extract_target_buf));

    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    extract_ret = cre2_extract(extract_pattern, &extract_text, &extract_rewrite, &extract_target);
    replace_ret = cre2_global_replace(replace_pattern, &replace_text_and_target, &replace_rewrite);

    // step 5: Validate
    computed_check = match_ret + extract_ret + replace_ret + (int)match_arr[0].length + extract_text.length + extract_target.length + replace_text_and_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}