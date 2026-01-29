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
//<ID> 1918
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha)(beta)?(?P<name>gamma)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * re_pat = NULL;
    int cap_groups = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup (Initialize and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_literal(opt, 0);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations (Compile regex and create named-groups iterator)
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    err_code = cre2_error_code(re);
    cap_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate (simple checksum to touch results)
    checksum = (long)pattern_len + (long)enc + (long)err_code + (long)cap_groups + (long)(re != NULL) + (long)(iter != NULL) + (long)(re_pat != NULL);
    (void)checksum;

    // step 5: Cleanup (release iterator, regex and options)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}