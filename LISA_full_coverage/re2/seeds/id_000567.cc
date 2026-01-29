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
//<ID> 567
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
    const char subject_buf[] = "xxfoobaryy";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t subject;
    cre2_string_t pmatches[3];
    int nmatch_pm = 3;
    int set_match_array[8];
    size_t set_match_count = 0;
    int dotnl_flag = 0;
    int rc_partial = 0;
    int rc_add = 0;
    int rc_compile = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&subject, 0, sizeof(subject));
    memset(pmatches, 0, sizeof(pmatches));
    memset(set_match_array, 0, sizeof(set_match_array));
    subject.data = subject_buf;
    subject.length = subject_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    dotnl_flag = cre2_opt_dot_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);

    // step 4: Operate
    rc_partial = cre2_partial_match(pattern, &subject, pmatches, nmatch_pm);
    set_match_count = cre2_set_match(set, subject_buf, (size_t)subject_len, set_match_array, (size_t)(sizeof(set_match_array) / sizeof(set_match_array[0])));

    // step 5: Validate
    computed_check = rc_partial + dotnl_flag + (int)set_match_count + pmatches[0].length + pmatches[1].length + rc_add + rc_compile + subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}