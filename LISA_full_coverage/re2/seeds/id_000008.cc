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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    cre2_options_t * opt = cre2_opt_new();
    const char * pattern = "a(b)c";
    int pattern_len = 5;
    const char * text_lit = "abc";
    int text_len = 3;
    cre2_regexp_t * re = 0;
    cre2_string_t match_arr[2];
    cre2_string_t submatches[2];
    cre2_range_t ranges[2];
    cre2_string_t text_str;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    memset(match_arr, 0, sizeof(match_arr));
    memset(submatches, 0, sizeof(submatches));
    memset(ranges, 0, sizeof(ranges));
    memset(&text_str, 0, sizeof(text_str));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));

    // step 2: Setup options and compile
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 0);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Matching operations
    match_arr[0].data = text_lit;
    match_arr[0].length = text_len;
    match_arr[1].data = 0;
    match_arr[1].length = 0;
    cre2_easy_match(pattern, pattern_len, text_lit, text_len, match_arr, 2);
    text_str.data = text_lit;
    text_str.length = text_len;
    cre2_full_match_re(re, &text_str, submatches, 2);

    // step 4: Convert matched strings to ranges and perform replacement
    cre2_strings_to_ranges(text_lit, ranges, submatches, 2);
    text_and_target.data = "abc abc";
    text_and_target.length = 7;
    rewrite_str.data = "Z";
    rewrite_str.length = 1;
    cre2_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}