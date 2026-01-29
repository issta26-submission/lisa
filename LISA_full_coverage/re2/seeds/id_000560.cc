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
//<ID> 560
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
    const char subject_buf[] = "xxfoobarZZ";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    int pm_res = 0;
    int dotnl_flag = 0;
    int set_matches[4];
    size_t set_matched_count = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(set_matches, 0, sizeof(set_matches));
    memset(&text, 0, sizeof(text));
    text.data = subject_buf;
    text.length = subject_len;
    opt = cre2_opt_new();

    // step 3: Configure
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 4: Operate (create regexp, run partial match)
    re = cre2_new(pattern, pattern_len, opt);
    pm_res = cre2_partial_match(pattern, &text, matches, 3);

    // step 5: Operate (create set, add pattern, compile, run set match)
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_matched_count = cre2_set_match(set, subject_buf, (size_t)subject_len, set_matches, 4);

    // step 6: Validate & Cleanup
    computed_check = dotnl_flag + pm_res + matches[0].length + matches[1].length + (int)set_matched_count + set_matches[0];
    (void)computed_check;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}