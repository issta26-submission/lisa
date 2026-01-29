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
//<ID> 1911
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
    const char * group_name = NULL;
    int group_index = -1;
    int err_code = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 3: Core operations
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &group_name, &group_index);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)err_code + (long)(re != NULL) + (long)(iter != NULL) + (long)(group_name != NULL) + (long)group_index;
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}