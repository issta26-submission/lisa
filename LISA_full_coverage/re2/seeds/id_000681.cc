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
//<ID> 681
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    struct cre2_set * set = NULL;
    const char * got_pattern = NULL;
    int add_rc = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&add_rc, 0, sizeof(add_rc));
    memset(&num_groups, 0, sizeof(num_groups));
    memset(&computed_check, 0, sizeof(computed_check));
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    got_pattern = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_rc = cre2_set_add_simple(set, pattern);

    // step 5: Validate
    num_groups = cre2_num_capturing_groups(re);
    computed_check = add_rc + num_groups + (int)got_pattern[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}