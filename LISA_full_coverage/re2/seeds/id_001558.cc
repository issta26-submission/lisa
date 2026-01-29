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
//<ID> 1558
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
    const char * ng_name = NULL;
    int ng_index = 0;
    int set_add_ret = 0;
    int compile_ret = 0;
    int group_count = 0;
    bool iter_next_ret = false;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure and operate on set and regexp
    set_add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Inspect regexp and named groups
    group_count = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    iter_next_ret = cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    cre2_named_groups_iter_delete(iter);
    iter = NULL;

    // step 5: Validate (compute a simple checksum to use results)
    checksum = (long)pattern_len + (long)set_add_ret + (long)compile_ret + (long)group_count
               + (long)(ng_name != NULL) + (long)ng_index + (long)iter_next_ret;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}