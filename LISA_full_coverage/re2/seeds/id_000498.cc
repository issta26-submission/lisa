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
//<ID> 498
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
    char target_buf[128];
    char text_target_buf[] = "foo bar foo";
    char rewrite_buf[] = "[$1]";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_str;
    cre2_string_t text_target;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    int nmatch = 4;
    int match_ret = 0;
    int ext_ret = 0;
    int repl_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(target_buf, 0, sizeof(target_buf));
    text_str.data = text_buf;
    text_str.length = text_len;
    text_target.data = text_target_buf;
    text_target.length = (int)(sizeof(text_target_buf) - 1);
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    target_str.data = target_buf;
    target_str.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_posix_syntax(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    ext_ret = cre2_extract(pattern, &text_str, &rewrite_str, &target_str);
    repl_ret = cre2_global_replace(pattern, &text_target, &rewrite_str);

    // step 5: Validate
    computed_check = match_ret + ext_ret + repl_ret + (int)match_arr[0].length + text_target.length + target_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}