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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)baz$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "apple|orange|banana";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int wb_flag = 0;
    int add_res = 0;
    int num_groups = 0;
    long computed_check = 0;
    char scratch[16];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    wb_flag = cre2_opt_word_boundary(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    set = cre2_set_new(opt, CRE2_ANCHOR_START);
    add_res = cre2_set_add_simple(set, set_pattern);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)wb_flag + (long)add_res + (long)num_groups + (long)scratch[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}