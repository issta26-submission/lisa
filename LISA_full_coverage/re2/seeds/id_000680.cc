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
//<ID> 680
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
    char text_buf[] = "zz foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    int match_indices[4];
    size_t set_matches = 0;
    const char * got_pattern = NULL;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);
    got_pattern = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Operate
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)4);

    // step 5: Validate
    computed_check = (int)set_matches + match_indices[0] + num_groups + (int)(got_pattern ? got_pattern[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}