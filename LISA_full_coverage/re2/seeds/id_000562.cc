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
//<ID> 562
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
    const char subject_buf[] = "xxfooYY";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t subject_str;
    cre2_string_t matches[3];
    int nmatch = 3;
    int partial_rc = 0;
    int dotnl_flag = 0;
    size_t set_matches = 0;
    int match_indices[4];
    size_t match_len = 4;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(match_indices, 0, sizeof(match_indices));
    subject_str.data = subject_buf;
    subject_str.length = subject_len;
    opt = cre2_opt_new();

    // step 3: Configure
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_rc = cre2_partial_match(pattern, &subject_str, matches, nmatch);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_matches = cre2_set_match(set, subject_buf, (size_t)subject_len, match_indices, match_len);

    // step 5: Validate
    computed_check = partial_rc + dotnl_flag + matches[0].length + (int)set_matches + match_indices[0] + (int)subject_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}