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
//<ID> 1497
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
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int word_boundary_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    iter = NULL;
    re = NULL;
    set = NULL;
    set_add_ret = 0;
    set_compile_ret = 0;
    word_boundary_flag = 0;
    checksum = 0;

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    /* cre2_set_new expects a cre2_anchor_t for the second argument.
       Use an explicit cast from 0 to cre2_anchor_t to avoid type mismatch. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 5: Validate
    checksum = (long)pattern_len + (long)word_boundary_flag + (long)set_add_ret + (long)set_compile_ret
               + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}