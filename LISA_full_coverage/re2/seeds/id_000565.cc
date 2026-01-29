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
//<ID> 565
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
    cre2_string_t subject;
    cre2_string_t matches[3];
    int match_indices[4];
    int nmatch = 3;
    size_t set_match_count = 0;
    int partial_rc = 0;
    int dot_nl_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&subject, 0, sizeof(subject));
    memset(matches, 0, sizeof(matches));
    memset(match_indices, 0, sizeof(match_indices));
    subject.data = subject_buf;
    subject.length = subject_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_dot_nl(opt, 1);
    dot_nl_flag = cre2_opt_dot_nl(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_rc = cre2_partial_match(pattern, &subject, matches, nmatch);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_match_count = cre2_set_match(set, subject_buf, (size_t)subject_len, match_indices, sizeof(match_indices) / sizeof(match_indices[0]));

    // step 5: Validate
    computed_check = partial_rc + dot_nl_flag + (int)set_match_count + matches[0].length + matches[1].length + match_indices[0] + subject_len + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}