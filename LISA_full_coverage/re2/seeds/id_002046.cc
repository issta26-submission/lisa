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
//<ID> 2046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int wb_before = 0;
    int wb_after = 0;
    cre2_encoding_t enc_before = CRE2_UNKNOWN;
    cre2_encoding_t enc_after = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup - create options and inspect/set word-boundary and encoding
    opt = cre2_opt_new();
    wb_before = cre2_opt_word_boundary(opt);
    enc_before = cre2_opt_encoding(opt);
    cre2_opt_set_word_boundary(opt, wb_before ? 0 : 1);
    wb_after = cre2_opt_word_boundary(opt);
    enc_after = cre2_opt_encoding(opt);

    // step 3: Operate - compile regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new((const cre2_regexp_t *)re);

    // step 4: Validate - produce a checksum flowing through collected values
    checksum = (long)pattern_len + (long)wb_before + (long)wb_after + (long)enc_before + (long)enc_after + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup - delete iterator, regexp and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}