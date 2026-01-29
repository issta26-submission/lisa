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
//<ID> 169
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
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1-$2";
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    cre2_range_t ranges[4];
    int easy_res = 0;
    int full_res = 0;
    int replace_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&ranges, 0, sizeof(ranges));
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 4: Operate - quick match, compile, convert matches to ranges, full match, then global replace
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    re = cre2_new(pattern, pattern_len, opt);
    cre2_strings_to_ranges(input_text, ranges, &matches[0], nmatch);
    full_res = cre2_full_match(pattern, &text_and_target, &matches[0], nmatch);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results (use values to influence state; no branching)
    text_and_target.length = text_and_target.length + ((easy_res + full_res + replace_res) & 7);
    cre2_opt_set_one_line(opt, (easy_res + replace_res) & 1);
    cre2_opt_set_case_sensitive(opt, (full_res >> 1) & 1);
    ranges[0].start = ranges[0].start + (easy_res & 3);
    (void)matches;
    (void)rewrite;
    (void)input_text_len;
    (void)pattern_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}