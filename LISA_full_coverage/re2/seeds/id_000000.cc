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
//<ID> 0
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
    const char * easy_pattern = "ab";
    int easy_pattern_len = 2;
    const char * text_cstr = "abc";
    int text_len = 3;
    cre2_regexp_t * rex = cre2_new(pattern, pattern_len, opt);
    cre2_string_t text_str = { text_cstr, text_len };
    cre2_string_t easy_match_arr[2];
    memset(easy_match_arr, 0, sizeof(easy_match_arr));
    int easy_match_ret = cre2_easy_match(easy_pattern, easy_pattern_len, text_cstr, text_len, easy_match_arr, 2);
    // step 2: Configure & inspect compiled regexp
    int n_groups = cre2_num_capturing_groups(rex);
    int nmatch = n_groups + 1;
    cre2_string_t * full_matches = (cre2_string_t *)malloc(sizeof(cre2_string_t) * nmatch);
    memset(full_matches, 0, sizeof(cre2_string_t) * nmatch);
    // step 3: Operate - full match using compiled regexp
    int full_match_ret = cre2_full_match_re(rex, &text_str, full_matches, nmatch);
    // step 4: Operate - convert matched strings to ranges and perform a replacement
    cre2_range_t * ranges = (cre2_range_t *)malloc(sizeof(cre2_range_t) * nmatch);
    memset(ranges, 0, sizeof(cre2_range_t) * nmatch);
    cre2_strings_to_ranges(text_str.data, ranges, full_matches, nmatch);
    cre2_string_t text_and_target = { text_cstr, text_len };
    cre2_string_t rewrite = { "X", 1 };
    int replace_ret = cre2_replace_re(rex, &text_and_target, &rewrite);
    // step 5: Cleanup
    free(full_matches);
    free(ranges);
    cre2_delete(rex);
    cre2_opt_delete(opt);
    (void)easy_match_ret;
    (void)full_match_ret;
    (void)replace_ret;
    return 66; // API sequence test completed successfully
}