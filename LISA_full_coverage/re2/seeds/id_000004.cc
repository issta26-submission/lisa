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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations and initialization
    cre2_options_t * opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    const char pattern_literal[] = "([0-9]+)";
    int pattern_len = (int)(sizeof(pattern_literal) - 1);
    const char text_literal[] = "abc 123 def 456";
    int text_len = (int)(sizeof(text_literal) - 1);
    const char rewrite_literal[] = "NUM";
    int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    cre2_regexp_t * rex = cre2_new(pattern_literal, pattern_len, opt);
    cre2_string_t matches[2];
    memset(matches, 0, sizeof(matches));
    cre2_range_t ranges[2];
    memset(ranges, 0, sizeof(ranges));

    // step 2: Easy match to find a numeric group
    cre2_easy_match(pattern_literal, pattern_len, text_literal, text_len, matches, 2);

    // step 3: Convert the found match strings into ranges relative to the original text
    cre2_strings_to_ranges(text_literal, ranges, matches, 2);

    // step 4: Use the first captured group as input for a full-match against the compiled regex
    cre2_string_t full_input = matches[1];
    cre2_string_t full_matches[2];
    memset(full_matches, 0, sizeof(full_matches));
    cre2_full_match_re(rex, &full_input, full_matches, 2);

    // step 5: Perform a replacement on the original text using the compiled regex
    cre2_string_t text_and_target;
    text_and_target.data = text_literal;
    text_and_target.length = text_len;
    cre2_string_t rewrite;
    rewrite.data = rewrite_literal;
    rewrite.length = rewrite_len;
    cre2_replace_re(rex, &text_and_target, &rewrite);

    // step 6: Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}