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
//<ID> 735
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
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int rc_match = 0;
    int rc_set_add = 0;
    int rc_set_compile = 0;
    int num_caps = 0;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    cre2_string_t * matches = NULL;
    int nmatch = 0;
    int match_indices[8];
    size_t set_matches = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    encoding = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    num_caps = cre2_num_capturing_groups(re);
    nmatch = num_caps + 1;
    matches = new cre2_string_t[(nmatch > 0) ? nmatch : 1];
    memset(matches, 0, (size_t)((nmatch > 0 ? nmatch : 1) * sizeof(cre2_string_t)));
    memset(match_indices, 0, sizeof(match_indices));
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_set_add = cre2_set_add_simple(set, pattern);
    rc_set_compile = cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)8);

    // step 5: Validate
    computed_check = rc_match + rc_set_add + rc_set_compile + num_caps + (int)set_matches + (int)encoding + matches[0].length + match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);
    delete [] matches;

    // API sequence test completed successfully
    return 66;
}