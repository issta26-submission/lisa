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
//<ID> 2043
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(?P<name>[A-Za-z]+)([0-9]{1,3})$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int initial_word_boundary = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int num_groups = 0;
    char scratch[32];
    long checksum = 0;

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    initial_word_boundary = cre2_opt_word_boundary(opt);
    cre2_opt_set_word_boundary(opt, initial_word_boundary ? 0 : 1);
    enc = cre2_opt_encoding(opt);
    memset(scratch, 0, sizeof(scratch));

    // step 3: Operate - compile regexp and create named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate - compute a checksum that flows through API results
    checksum = (long)pattern_len + (long)initial_word_boundary + (long)enc + (long)(re != NULL) + (long)num_groups + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup - delete iterator, regexp and options
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}