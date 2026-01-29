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
//<ID> 564
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
    const char subject_buf[] = "xxfoo\nbarYY";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t input_str;
    cre2_string_t match_out[3];
    int match_indices[4];
    int nmatch = 3;
    int rc_partial = 0;
    size_t set_matches = 0;
    int dotnl_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&input_str, 0, sizeof(input_str));
    memset(match_out, 0, sizeof(match_out));
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();

    // step 3: Configure / initialize regex and set
    dotnl_flag = cre2_opt_dot_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate using partial match and set match
    input_str.data = subject_buf;
    input_str.length = subject_len;
    rc_partial = cre2_partial_match(pattern, &input_str, match_out, nmatch);
    set_matches = cre2_set_match(set, subject_buf, (size_t)subject_len, match_indices, (size_t)4);

    // step 5: Validate (aggregate results to avoid unused warnings)
    computed_check = dotnl_flag + rc_partial + (int)set_matches + match_out[0].length + match_out[1].length + match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}