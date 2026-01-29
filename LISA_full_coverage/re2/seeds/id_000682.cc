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
//<ID> 682
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
    char text[] = "zz foobar xx";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * got_pattern = NULL;
    int add_rc = 0;
    int compile_rc = 0;
    size_t match_count = 0;
    int matches[8];
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    add_rc = cre2_set_add_simple(set, pattern);
    compile_rc = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    got_pattern = cre2_pattern(re);

    // step 4: Operate
    match_count = cre2_set_match(set, text, text_len, matches, (size_t)(sizeof(matches) / sizeof(matches[0])));
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate
    computed_check = add_rc + compile_rc + (int)match_count + num_groups + pattern_len + (int)(got_pattern ? got_pattern[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}