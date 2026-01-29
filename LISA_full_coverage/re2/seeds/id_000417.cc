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
//<ID> 417
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
    const char set_pattern[] = "hello|foo";
    const int set_pattern_len = (int)(sizeof(set_pattern) - 1);
    const char text_buf[] = "xxfoofoobarzzhello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t matches[3];
    int match_indices[4];
    size_t set_matches = 0;
    int longest_flag = 0;
    const char *pattern_from_re = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_longest_match(opt, 1);
    longest_flag = cre2_opt_longest_match(opt);
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    cre2_set_add_simple(set, pattern_from_re);
    cre2_set_add_simple(set, set_pattern);

    // step 4: Operate
    cre2_set_compile(set);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)4);
    cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);

    // step 5: Validate
    computed_check = (int)set_matches + longest_flag + matches[0].length + match_indices[0] + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}