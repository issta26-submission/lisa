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
//<ID> 919
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char easy_pattern[] = "([0-9]+)";
    const int easy_pattern_len = (int)(sizeof(easy_pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_init[] = "number 12345 and more 67890";
    char text_buf[128] = "number 12345 and more 67890";
    const char rewrite_init[] = "REPLACED";
    char rewrite_buf[128] = "REPLACED";
    const int text_len = (int)(sizeof(text_init) - 1);
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_res = 0;
    int match_res = 0;
    int replace_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    memset(match_arr, 0, sizeof(match_arr));
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, match_arr, 2);
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 3);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_init) - 1);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)easy_res + (long)match_res + (long)match_arr[0].length + (long)match_arr[1].length + (long)replace_res + (long)text_and_target.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}