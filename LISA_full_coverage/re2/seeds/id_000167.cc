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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "xx abcd yy ab cd";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1_$2";
    const int nmatch = 3;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t full_matches[4];
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    cre2_range_t ranges[4];
    int easy_res = 0;
    int full_res = 0;
    int replace_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&full_matches, 0, sizeof(full_matches));
    memset(&ranges, 0, sizeof(ranges));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&text_and_target, 0, sizeof(text_and_target));
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    text_and_target.data = input_text;
    text_and_target.length = input_text_len;

    // step 3: Core operations (quick match and full match)
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    full_res = cre2_full_match(pattern, &text_and_target, &full_matches[0], nmatch);

    // step 4: Convert matched strings to ranges, compile and perform global replace with compiled regexp
    cre2_strings_to_ranges(input_text, ranges, &matches[0], nmatch);
    re = cre2_new(pattern, pattern_len, NULL);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results (use values to influence state; no branching)
    text_and_target.length = text_and_target.length + (easy_res & 7) + (full_res & 3) + (replace_res & 1);
    full_matches[0].length = full_matches[0].length + (int)(ranges[0].past - ranges[0].start);
    matches[0].length = matches[0].length + (replace_res & 3);
    (void)pattern_len;
    (void)input_text_len;
    (void)matches;
    (void)full_matches;
    (void)ranges;
    (void)rewrite;
    (void)text_and_target;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}