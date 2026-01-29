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
//<ID> 563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject_buf[] = "zabcx";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text;
    cre2_string_t matches[3];
    int nmatch = 3;
    int partial_rc = 0;
    size_t set_rc = 0;
    int add_rc = 0;
    int compile_rc = 0;
    int dotnl_flag = 0;
    int match_indices[4];
    int computed_check = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(matches, 0, sizeof(matches));
    memset(match_indices, -1, sizeof(match_indices));
    opt = cre2_opt_new();

    // step 3: Configure
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 4: Operate
    text.data = subject_buf;
    text.length = subject_len;
    re = cre2_new(pattern, pattern_len, opt);
    partial_rc = cre2_partial_match(pattern, &text, matches, nmatch);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_rc = cre2_set_add_simple(set, pattern);
    compile_rc = cre2_set_compile(set);
    set_rc = cre2_set_match(set, subject_buf, (size_t)subject_len, match_indices, (size_t)(sizeof(match_indices) / sizeof(match_indices[0])));

    // step 5: Validate
    computed_check = partial_rc + dotnl_flag + add_rc + compile_rc + (int)set_rc + matches[0].length + (int)text.length + match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}