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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char *pattern = "world";
    int pattern_len = 5;
    const char *text = "hello world hello world";
    int text_len = 23;
    cre2_string_t easy_match_res[2];
    memset(easy_match_res, 0, sizeof(easy_match_res));
    // step 2: Easy match using pattern string
    int easy_ret = cre2_easy_match(pattern, pattern_len, text, text_len, easy_match_res, 2);
    // step 3: Convert matched strings to ranges
    cre2_range_t ranges[2];
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text, ranges, easy_match_res, 2);
    // step 4: Compile regex and perform a replace using compiled regex
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, (const cre2_options_t *)0);
    cre2_string_t text_and_target;
    text_and_target.data = text;
    text_and_target.length = text_len;
    cre2_string_t rewrite;
    rewrite.data = "WORLD";
    rewrite.length = 5;
    int replace_ret = cre2_replace_re(rex, &text_and_target, &rewrite);
    // step 5: Full match against the (possibly replaced) text using compiled regex
    cre2_string_t fm_matches[2];
    memset(fm_matches, 0, sizeof(fm_matches));
    int full_ret = cre2_full_match_re(rex, &text_and_target, fm_matches, 2);
    // step 6: Cleanup
    cre2_delete(rex);
    // API sequence test completed successfully
    (void)easy_ret;
    (void)replace_ret;
    (void)full_ret;
    return 66;
}