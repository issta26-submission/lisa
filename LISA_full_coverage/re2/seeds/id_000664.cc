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
//<ID> 664
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(f)(oo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t match;
    cre2_string_t strings[64];
    cre2_range_t ranges[64];
    int rc_easy = 0;
    int num_groups = 0;
    int nmatch = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile and iterate
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Match
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 5: Convert & Validate
    strings[0] = match;
    nmatch = num_groups + 1;
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);
    computed_check = rc_easy + match.length + num_groups + (int)ranges[0].start + (int)ranges[0].past;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}