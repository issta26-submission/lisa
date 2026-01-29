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
//<ID> 2018
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)[0-9]*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    const char * pattern_from_re = NULL;
    int log_flag = 0;
    int add_ret = 0;
    int set_compile_ret = 0;
    long checksum = 0;

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_flag = cre2_opt_log_errors(opt);

    // step 3: Operate - compile regexp, create iterator, build a set with the same options
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Validate - compute a simple checksum that depends on results
    checksum = (long)pattern_len + (long)log_flag + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL) + (long)add_ret + (long)set_compile_ret + (long)(pattern_from_re != NULL);
    (void)checksum;

    // step 5: Cleanup - free resources
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}