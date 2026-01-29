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
//<ID> 911
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char easy_pattern[] = "(ab)(c)";
    const int easy_pattern_len = (int)(sizeof(easy_pattern) - 1);
    const char text[] = "abcabc";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t rewrite;
    char target_buf[128] = "abcabc";
    cre2_string_t text_and_target;
    int easy_res = 0;
    int match_res = 0;
    int replace_res = 0;
    int one_line_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(match_arr, 0, sizeof(match_arr));
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text, text_len, &match_arr[0], 4);
    match_res = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, &match_arr[0], 4);
    rewrite.data = "X";
    rewrite.length = 1;
    text_and_target.data = target_buf;
    text_and_target.length = (int)(sizeof("abcabc") - 1);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)one_line_flag + (long)easy_res + (long)match_res + (long)replace_res + (long)text_and_target.length + (long)rewrite.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}