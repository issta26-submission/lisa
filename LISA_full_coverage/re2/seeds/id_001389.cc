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
//<ID> 1389
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)|bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * group_name = NULL;
    int group_index = -1;
    bool next_ret = false;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int literal_flag = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    enc = cre2_opt_encoding(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 4: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    next_ret = cre2_named_groups_iter_next(iter, &group_name, &group_index);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)enc + (long)literal_flag
                     + (long)next_ret + (long)group_index + (long)num_groups
                     + (long)(group_name != NULL) + (long)(re != NULL)
                     + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;
    (void)group_name;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}