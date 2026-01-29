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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "test";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "this is test and more test";
    const int text_len = (int)(sizeof(text) - 1);
    const char replacement[] = "X";
    const int replacement_len = (int)(sizeof(replacement) - 1);
    cre2_regexp_t * re = cre2_new(pattern, pattern_len, NULL);
    cre2_string_t easy_match;
    cre2_range_t ranges[1];
    cre2_string_t strings_for_ranges[1];
    cre2_string_t full_input;
    cre2_string_t full_results[1];
    char buffer[128];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;

    // step 2: Setup (clear structures and prepare buffers)
    memset(&easy_match, 0, sizeof(easy_match));
    memset(ranges, 0, sizeof(ranges));
    memset(strings_for_ranges, 0, sizeof(strings_for_ranges));
    memset(&full_input, 0, sizeof(full_input));
    memset(full_results, 0, sizeof(full_results));
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, text, (size_t)text_len);
    text_and_target.data = buffer;
    text_and_target.length = text_len;
    rewrite.data = replacement;
    rewrite.length = replacement_len;

    // step 3: Operate (match using easy API)
    int easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &easy_match, 1);

    // step 4: Operate (convert matched strings to ranges)
    strings_for_ranges[0] = easy_match;
    cre2_strings_to_ranges(text, ranges, strings_for_ranges, 1);

    // step 5: Operate (full match against compiled regexp)
    full_input.data = text;
    full_input.length = text_len;
    int full_res = cre2_full_match_re(re, &full_input, full_results, 1);

    // step 6: Operate (perform a replace using compiled regexp) and Cleanup
    int replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    cre2_delete(re);

    // API sequence test completed successfully
    (void)easy_res;
    (void)full_res;
    (void)replace_res;
    (void)ranges;
    (void)full_results;
    return 66;
}