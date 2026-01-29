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
//<ID> 1914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    bool iter_has = false;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int err_code = 0;
    char scratch[8];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_literal(opt, 0);
    enc = cre2_opt_encoding(opt);
    memset(scratch, 0, sizeof(scratch));
    scratch[0] = (char)enc;

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    iter = cre2_named_groups_iter_new(re);
    iter_has = cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 4: Validate
    checksum = (long)pattern_len + (long)err_code + (long)enc + (long)iter_has + (long)found_index + (long)(found_name != NULL) + (long)(scratch[0] != 0) + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}