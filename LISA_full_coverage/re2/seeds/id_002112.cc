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
//<ID> 2112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    char set_err[128];
    int num_groups = 0;
    int err_code = 0;
    int set_add_result = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    memset(set_err, 0, sizeof(set_err));

    // step 3: Configure (compile regexp using options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect regexp and add pattern to a set)
    num_groups = cre2_num_capturing_groups(re);
    err_code = cre2_error_code(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_result = cre2_set_add(set, pattern, (size_t)pattern_len, set_err, sizeof(set_err));

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)num_groups + (long)err_code + (long)set_add_result + (long)(unsigned char)set_err[0] + (long)(re != NULL) + (long)(opt != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}