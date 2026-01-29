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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b)";
    const int pattern_len = 4;
    const char text_buf[] = "zabz";
    const int text_len = 4;
    cre2_options_t * opt = (cre2_options_t *)0;
    cre2_regexp_t * rex = (cre2_regexp_t *)0;
    cre2_string_t match_arr[2];
    cre2_range_t ranges[2];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t fullmatch_arr[2];
    int easy_res = 0;
    int replace_res = 0;
    int full_res = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    rex = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate - easy match and convert matched strings to ranges
    memset(match_arr, 0, sizeof(match_arr));
    memset(ranges, 0, sizeof(ranges));
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, 2);
    cre2_strings_to_ranges(text_buf, ranges, match_arr, 2);

    // step 4: Operate - perform a replacement using compiled regex
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "Q";
    rewrite.length = 1;
    replace_res = cre2_replace_re(rex, &text_and_target, &rewrite);

    // step 5: Validate - attempt a full match on the target string and collect captures
    memset(fullmatch_arr, 0, sizeof(fullmatch_arr));
    full_res = cre2_full_match_re(rex, &text_and_target, fullmatch_arr, 2);

    // step 6: Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);
    puts("API sequence test completed successfully");
    (void)easy_res;
    (void)replace_res;
    (void)full_res;
    (void)ranges;
    (void)match_arr;
    (void)fullmatch_arr;
    return 66;
}