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
//<ID> 2116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo)(bar)?$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    char set_error[64];
    int err_code = 0;
    int num_groups = 0;
    int add_result = 0;
    int compile_result = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure (compile regexp using options and inspect)
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Operate (create a set, add pattern and compile the set)
    /* cre2_set_new expects a cre2_anchor_t for the second argument.
       Use an explicit cast of 0 to cre2_anchor_t to match the expected type. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    memset(set_error, 0, sizeof(set_error));
    add_result = cre2_set_add(set, pattern, (size_t)pattern_len, set_error, sizeof(set_error));
    compile_result = cre2_set_compile(set);

    // step 5: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)err_code + (long)num_groups + (long)add_result + (long)compile_result + (long)(re != NULL) + (long)(set != NULL) + (long)set_error[0];
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}