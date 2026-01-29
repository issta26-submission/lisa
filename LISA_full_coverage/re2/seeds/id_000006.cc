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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char * pattern = "a(b)c";
    int pattern_len = 5;
    const char * text = "abc abc";
    int text_len = 7;
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * rex = cre2_new(pattern, pattern_len, opt);
    cre2_string_t match_easy[2];
    cre2_string_t match_full[2];
    cre2_range_t ranges[2];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;

    // step 2: Setup
    cre2_opt_set_case_sensitive(opt, 1);
    match_easy[0].data = 0; match_easy[0].length = 0;
    match_easy[1].data = 0; match_easy[1].length = 0;
    match_full[0].data = 0; match_full[0].length = 0;
    match_full[1].data = 0; match_full[1].length = 0;
    ranges[0].start = 0; ranges[0].past = 0;
    ranges[1].start = 0; ranges[1].past = 0;
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;

    // step 3: Operate - easy match and convert matches to ranges
    cre2_easy_match(pattern, pattern_len, text, text_len, match_easy, 2);
    cre2_strings_to_ranges(text, ranges, match_easy, 2);

    // step 4: Operate - perform replace using compiled regex and then full match using compiled regex
    cre2_replace_re(rex, &text_and_target, &rewrite);
    cre2_full_match_re(rex, &text_and_target, match_full, 2);

    // step 5: Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);
    return 66;
    // API sequence test completed successfully
}