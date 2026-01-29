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
//<ID> 957
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * extracted_pattern = NULL;
    int num_groups = 0;
    long computed_check = 0L;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 0);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    extracted_pattern = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)num_groups + (long)pattern_len + (long)(extracted_pattern != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}