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
//<ID> 411
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
    const char text_buf[] = "xxfoofoobarzzhello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_sp;
    int match_indices[4];
    size_t set_matches = 0;
    int easy_res = 0;
    int longest_flag = 0;
    const char *patptr = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(match_indices, 0, sizeof(match_indices));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_longest_match(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    longest_flag = cre2_opt_longest_match(opt);
    patptr = cre2_pattern(re);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)4);

    // step 5: Validate
    computed_check = easy_res + longest_flag + (int)set_matches + matches[0].length + matches[1].length + match_indices[0] + pattern_len + text_len + (int)patptr[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}