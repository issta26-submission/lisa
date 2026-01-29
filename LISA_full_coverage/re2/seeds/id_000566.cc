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
//<ID> 566
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern_simple[] = "foo";
    const char text[] = "xxfooYY";
    const int text_len = (int)(sizeof(text) - 1);

    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;

    cre2_string_t subject;
    cre2_string_t matches[2];
    int nmatch = 2;

    int match_idx[4];
    size_t set_rc = 0;
    int partial_rc = 0;
    int dot_nl_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&subject, 0, sizeof(subject));
    memset(matches, 0, sizeof(matches));
    memset(match_idx, 0, sizeof(match_idx));
    opt = cre2_opt_new();

    // step 3: Configure
    dot_nl_flag = cre2_opt_dot_nl(opt);
    subject.data = text;
    subject.length = text_len;

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_rc = cre2_partial_match(pattern, &subject, matches, nmatch);
    // cast 0 to cre2_anchor_t because this code is compiled as C/C++
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, set_pattern_simple);
        cre2_set_compile(set);
        set_rc = cre2_set_match(set, text, (size_t)text_len, match_idx, (size_t)(sizeof(match_idx)/sizeof(match_idx[0])));
    }

    // step 5: Validate
    computed_check = partial_rc + (int)set_rc + dot_nl_flag + matches[0].length + matches[1].length + subject.length + match_idx[0];
    (void)computed_check;

    // step 6: Cleanup
    if (set) cre2_set_delete(set);
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}