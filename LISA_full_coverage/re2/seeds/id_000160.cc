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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab(c))d?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "xx abcd yy";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1-$3";
    const int nmatch = 5;

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;

    cre2_string_t matches[6];
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    cre2_range_t ranges[6];

    int easy_res = 0;
    int full_res = 0;
    int global_replace_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&ranges, 0, sizeof(ranges));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&text_and_target, 0, sizeof(text_and_target));

    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);

    text_and_target.data = input_text;
    text_and_target.length = input_text_len;

    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations (quick match, full match, convert ranges, compile, global replace)
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    re = cre2_new(pattern, pattern_len, opt);
    full_res = cre2_full_match(pattern, &text_and_target, &matches[0], nmatch);
    cre2_strings_to_ranges(text_and_target.data, ranges, &matches[0], nmatch);
    global_replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / propagate results (use values to influence state; no branching)
    text_and_target.length = text_and_target.length + ((easy_res + full_res + global_replace_res) & 7);
    cre2_opt_set_one_line(opt, (global_replace_res + easy_res) & 1);
    cre2_opt_set_case_sensitive(opt, (full_res >> 1) & 1);
    (void)matches;
    (void)ranges;
    (void)rewrite;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}