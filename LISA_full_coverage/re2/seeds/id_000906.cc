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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char re_pattern[] = "(?P<name>foo)bar";
    const int pattern_len = (int)(sizeof(re_pattern) - 1);
    const char set_pattern[] = "foo|bar";
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int word_boundary_flag = 0;
    int add_res = 0;
    const char *pattern_from_re = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_ANCHOR_START);
    add_res = cre2_set_add_simple(set, set_pattern);
    re = cre2_new(re_pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    pattern_from_re = cre2_pattern(re);
    computed_check = (long)pattern_len + (long)word_boundary_flag + (long)add_res + (long)pattern_from_re[0] + (long)iter;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}