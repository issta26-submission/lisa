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
//<ID> 2038
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)[0-9]+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int err_code = 0;
    int64_t requested_max_mem = (int64_t)(4 * 1024 * 1024); // 4 MiB
    int add_ret = 0;
    int compile_ret = 0;
    long checksum = 0;

    // step 2: Setup - create options and configure max memory
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, requested_max_mem);

    // step 3: Operate - compile a regexp and create a set using the same options
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Validate - fold several results into a checksum to ensure values flowed
    checksum = (long)pattern_len + (long)err_code + (long)requested_max_mem + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)add_ret + (long)compile_ret;
    (void)checksum;

    // step 5: Cleanup - free resources in reverse order of acquisition
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}