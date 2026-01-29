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
//<ID> 2034
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^([A-Za-z]+)-([0-9]{1,4})$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int rc_add = 0;
    int rc_compile = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup - create options and set memory limit
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)16 * 1024 * 1024);

    // step 3: Configure / Operate - create a set with the configured options and compile a regexp
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Validate - query error code and compute a checksum to ensure values propagated
    err_code = cre2_error_code(re);
    checksum = (long)pattern_len + (long)err_code + (long)rc_add + (long)rc_compile + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup - destroy created objects
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}