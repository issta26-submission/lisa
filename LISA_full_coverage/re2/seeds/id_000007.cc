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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    char pattern[] = "(foo)(\\d+)";
    int pat_len = (int)(sizeof(pattern) - 1);
    char sample_text[] = "foo123 bar";
    int sample_text_len = (int)(sizeof(sample_text) - 1);
    cre2_string_t match[3];
    cre2_range_t ranges[3];
    memset(match, 0, sizeof(match));
    memset(ranges, 0, sizeof(ranges));
    char full_text[] = "foo456";
    cre2_string_t full_text_str = { full_text, (int)(sizeof(full_text) - 1) };
    cre2_string_t full_match_results[3];
    memset(full_match_results, 0, sizeof(full_match_results));
    char replace_target_buf[] = "foo456";
    cre2_string_t text_and_target = { replace_target_buf, (int)(sizeof(replace_target_buf) - 1) };
    cre2_string_t rewrite = { "NUM:$2", (int)(sizeof("NUM:$2") - 1) };

    // step 2: Setup - compile regex
    cre2_regexp_t * rex = cre2_new(pattern, pat_len, (const cre2_options_t *)0);

    // step 3: Operate - easy match and convert to ranges
    cre2_easy_match(pattern, pat_len, sample_text, sample_text_len, match, 3);
    cre2_strings_to_ranges(sample_text, ranges, match, 3);

    // step 4: Operate - full match using compiled regex
    cre2_full_match_re(rex, &full_text_str, full_match_results, 3);

    // step 5: Operate - perform a replace using compiled regex
    cre2_replace_re(rex, &text_and_target, &rewrite);

    // step 6: Cleanup
    cre2_delete(rex);

    // API sequence test completed successfully
    return 66;
}