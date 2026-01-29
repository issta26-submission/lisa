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
//<ID> 161
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
    const char input_text[] = "xx foobar yy";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1_$2";
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_string;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_range_t ranges[4];
    int easy_res = 0;
    int full_res = 0;
    int replace_res = 0;
    int combined = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&text_string, 0, sizeof(text_string));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(ranges, 0, sizeof(ranges));
    text_string.data = input_text;
    text_string.length = input_text_len;
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations - quick match and compile
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations - full match and convert matched strings to ranges
    full_res = cre2_full_match(pattern, &text_string, &matches[0], nmatch);
    cre2_strings_to_ranges(input_text, ranges, &matches[0], nmatch);

    // step 5: Operate - global replace using compiled regexp and propagate results
    combined = (easy_res + full_res) & 7;
    text_and_target.length = text_and_target.length + combined;
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate / Cleanup (use values to influence state; no branching)
    (void)ranges;
    (void)matches;
    (void)easy_res;
    (void)full_res;
    (void)replace_res;
    (void)combined;
    (void)pattern_len;
    (void)input_text_len;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}