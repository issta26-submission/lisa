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
//<ID> 2048
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int wb_before = 0;
    int wb_after = 0;
    cre2_encoding_t enc_before = CRE2_UNKNOWN;
    int num_capturing = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup - create and inspect options
    opt = cre2_opt_new();
    wb_before = cre2_opt_word_boundary(opt);
    enc_before = cre2_opt_encoding(opt);
    cre2_opt_set_word_boundary(opt, 1);
    wb_after = cre2_opt_word_boundary(opt);

    // step 3: Operate - compile regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    num_capturing = cre2_num_capturing_groups(re);
    err_code = cre2_error_code(re);

    // step 4: Validate - flow values into a checksum to ensure usage
    checksum = (long)pattern_len + (long)wb_before + (long)wb_after + (long)enc_before + (long)num_capturing + (long)err_code + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup - delete iterator, regexp, and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}