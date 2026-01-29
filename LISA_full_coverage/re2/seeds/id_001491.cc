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
//<ID> 1491
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
    cre2_set * set = NULL;
    int opt_wb_before = 0;
    int opt_wb_after = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    opt_wb_before = cre2_opt_word_boundary(opt);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    opt_wb_after = cre2_opt_word_boundary(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);

    // step 4: Core operations
    set_compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)set_add_ret + (long)set_compile_ret
               + (long)opt_wb_before + (long)opt_wb_after + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}