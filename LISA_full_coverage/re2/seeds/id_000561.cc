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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(bc)?d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject[] = "xxabcdyy";
    const int subject_len = (int)(sizeof(subject) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text_arr[1];
    cre2_string_t matches[3];
    int match_indices[4];
    int dotnl_flag = 0;
    int partial_rc = 0;
    size_t set_rc = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(text_arr, 0, sizeof(text_arr));
    memset(matches, 0, sizeof(matches));
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_dot_nl(opt, 1);
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 4: Operate
    text_arr[0].data = subject;
    text_arr[0].length = subject_len;
    re = cre2_new(pattern, pattern_len, opt);
    partial_rc = cre2_partial_match(pattern, text_arr, matches, 3);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_rc = cre2_set_match(set, subject, (size_t)subject_len, match_indices, 4);

    // step 5: Validate
    computed_check = dotnl_flag + pattern_len + subject_len + partial_rc + (int)set_rc + matches[0].length + matches[1].length + match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}