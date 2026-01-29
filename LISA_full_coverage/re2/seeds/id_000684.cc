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
//<ID> 684
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
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    int match_indices[4];
    size_t matches_found = 0;
    const char * got_pattern = NULL;
    int rc_add = 0;
    int rc_compile = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Configure
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    re = cre2_new(pattern, pattern_len, opt);
    got_pattern = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    matches_found = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, 4);

    // step 5: Validate
    computed_check = (int)matches_found + rc_add + rc_compile + match_indices[0] + (int)got_pattern[0] + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}