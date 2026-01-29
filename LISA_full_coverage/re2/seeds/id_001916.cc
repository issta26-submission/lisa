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
//<ID> 1916
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *re_pat = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int err_code = 0;
    char scratch[64];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_literal(opt, 0);
    memset(scratch, 0, sizeof(scratch));

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate
    enc = cre2_opt_encoding(opt);
    re_pat = cre2_pattern(re);
    err_code = cre2_error_code(re);
    checksum = (long)pattern_len + (long)enc + (long)(re_pat != NULL) + (long)err_code + (long)(iter != NULL) + (long)(opt != NULL) + (long)sizeof(scratch);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}