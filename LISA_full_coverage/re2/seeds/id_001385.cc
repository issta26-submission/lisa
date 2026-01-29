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
//<ID> 1385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>hello)|world";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * group_name = NULL;
    int group_index = -1;
    int literal_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_ret = 0;
    int compile_ret = 0;
    bool iter_next_ret = false;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Query option state
    literal_flag = cre2_opt_literal(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Prepare set and compile
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 5: Create regexp and iterate named groups once
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    iter_next_ret = cre2_named_groups_iter_next(iter, &group_name, &group_index);

    // step 6: Validate / Cleanup
    computed_check = (long)pattern_len + (long)literal_flag + (long)enc
                     + (long)add_ret + (long)compile_ret
                     + (long)(re != NULL) + (long)(set != NULL) + (long)(iter != NULL)
                     + (long)group_index + (long)(group_name != NULL) + (long)iter_next_ret;
    (void)computed_check;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}