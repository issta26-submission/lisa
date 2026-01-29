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
//<ID> 2115
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
    cre2_set *set = NULL;
    char set_error[128];
    memset(set_error, 0, sizeof(set_error));
    int add_result = 0;
    int err_code = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure (compile a regexp using the options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect regexp and add pattern to a set)
    num_groups = re ? cre2_num_capturing_groups(re) : 0;
    err_code = re ? cre2_error_code(re) : 0;
    /* cre2_set_new expects a cre2_anchor_t for the second parameter.
       Preserve the original intent of passing 0 by casting to that type. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_result = cre2_set_add(set, pattern, (size_t)pattern_len, set_error, sizeof(set_error));

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)num_groups + (long)err_code + (long)add_result + (long)(re != NULL) + (long)(set != NULL) + (long)set_error[0];
    (void)checksum;

    // step 6: Cleanup (release resources)
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}