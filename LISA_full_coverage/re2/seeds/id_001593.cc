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
//<ID> 1593
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
    int case_sensitive = 0;
    int err_code = 0;
    int num_groups = 0;
    int ver_rev = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    case_sensitive = cre2_opt_case_sensitive(opt);
    cre2_opt_set_log_errors(opt, case_sensitive);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    err_code = cre2_error_code(re);
    num_groups = cre2_num_capturing_groups(re);
    ver_rev = cre2_version_interface_revision();

    // step 5: Validate
    checksum = (long)pattern_len + (long)case_sensitive + (long)err_code + (long)num_groups + (long)ver_rev
               + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}