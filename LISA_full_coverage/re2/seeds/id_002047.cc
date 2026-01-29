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
//<ID> 2047
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(?P<name>[A-Za-z]+)([0-9]+)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_encoding_t enc_before = CRE2_UNKNOWN;
    cre2_encoding_t enc_after = CRE2_UNKNOWN;
    int word_before = 0;
    int word_after = 0;
    int err_code = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup - create and inspect default options
    opt = cre2_opt_new();
    enc_before = cre2_opt_encoding(opt);
    word_before = cre2_opt_word_boundary(opt);

    // step 3: Configure - change word boundary option
    cre2_opt_set_word_boundary(opt, 1);

    // step 4: Operate - compile a regexp using the configured options
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate - create named-groups iterator and re-check options/encoding
    iter = cre2_named_groups_iter_new(re);
    word_after = cre2_opt_word_boundary(opt);
    enc_after = cre2_opt_encoding(opt);
    checksum = (long)pattern_len + (long)err_code + (long)num_groups + (long)word_before + (long)word_after + (long)enc_before + (long)enc_after + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}