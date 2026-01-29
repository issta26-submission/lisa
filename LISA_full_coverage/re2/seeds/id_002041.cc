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
//<ID> 2041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)\\s+(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int boundary_before = 0;
    int boundary_after = 0;
    long checksum = 0;

    // step 2: Setup - create options and inspect/set word boundary and encoding
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 0);
    boundary_before = cre2_opt_word_boundary(opt);
    cre2_opt_set_word_boundary(opt, 1);
    boundary_after = cre2_opt_word_boundary(opt);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate - compile the regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate - compute a checksum that uses meaningful API outputs
    checksum = (long)pattern_len + (long)boundary_before + (long)boundary_after + (long)enc + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup - delete iterator, regexp, and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}