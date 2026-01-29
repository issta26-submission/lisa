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
//<ID> 2062
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(\\w+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "user-42";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    int one_line_before = 0;
    int one_line_after = 0;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t matches[4];
    int match_result = 0;
    char target_buf[128];
    cre2_string_t text_and_target;
    char rewrite_buf[] = "$1-99";
    cre2_string_t rewrite;
    int add_result = 0;
    int compile_result = 0;
    int replace_result = 0;
    long checksum = 0;

    // step 2: Setup (options and one_line flag)
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);

    // step 3: Configure (build and populate set, compile, and build regexp)
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_result = cre2_set_add_simple(set, pattern);
    compile_result = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (perform match and prepare for replace)
    memset(matches, 0, sizeof(matches));
    match_result = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(target_buf, text, (size_t)text_len);
    text_and_target.data = target_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    replace_result = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (produce a checksum to tie values together)
    checksum = (long)pattern_len + (long)match_result + (long)replace_result + (long)one_line_before + (long)one_line_after + (long)matches[0].length + (long)add_result + (long)compile_result + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup (delete regexp, set and options)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}