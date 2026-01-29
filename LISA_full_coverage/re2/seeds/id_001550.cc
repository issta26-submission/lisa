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
//<ID> 1550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g1>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_named_groups_iter_t * iter = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    bool iter_next_ret = false;
    int add_ret = 0;
    int compile_ret = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    iter_next_ret = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)compile_ret + (long)num_groups
               + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL)
               + (long)found_index + (long)(int)iter_next_ret;
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}