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
//<ID> 1553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    struct cre2_named_groups_iter_t * iter = NULL;
    const char * group_name = NULL;
    int group_index = -1;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    set_compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    bool iter_has = cre2_named_groups_iter_next(iter, &group_name, &group_index);
    (void)iter_has;

    // step 5: Validate
    checksum = (long)pattern_len + (long)set_add_ret + (long)set_compile_ret + (long)num_groups
               + (long)(group_name != NULL) + (long)group_index + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}