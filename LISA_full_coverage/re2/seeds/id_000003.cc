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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations & initialization
    cre2_options_t * opt = cre2_opt_new();
    const char pattern[] = "(foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t * rex = cre2_new(pattern, pattern_len, opt);
    const char simple_pattern[] = "foo";
    const int simple_pattern_len = (int)(sizeof(simple_pattern) - 1);
    const char subject[] = "foobar foobar";
    const int subject_len = (int)(sizeof(subject) - 1);
    const char rewrite_text[] = "baz";
    const int rewrite_len = (int)(sizeof(rewrite_text) - 1);

    // step 2: Prepare cre2_string_t and buffers
    cre2_string_t input = { subject, subject_len };
    cre2_string_t text_and_target = { subject, subject_len };
    cre2_string_t rewrite = { rewrite_text, rewrite_len };
    cre2_string_t match;
    cre2_string_t out_strings[3];
    cre2_range_t ranges[3];
    memset(out_strings, 0, sizeof(out_strings));
    memset(ranges, 0, sizeof(ranges));

    // step 3: Quick easy match to populate a single match structure
    cre2_easy_match(simple_pattern, simple_pattern_len, subject, subject_len, &match, 1);

    // step 4: Full match using compiled regexp to populate capture strings
    cre2_full_match_re(rex, &input, out_strings, 3);

    // step 5: Convert obtained strings to ranges within the original text
    cre2_strings_to_ranges(subject, ranges, out_strings, 3);

    // step 6: Perform a replace operation using the compiled regexp and a rewrite
    cre2_replace_re(rex, &text_and_target, &rewrite);

    // Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}