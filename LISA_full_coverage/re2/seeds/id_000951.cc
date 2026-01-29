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
//<ID> 951
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
    const char * observed_pattern = NULL;
    int capturing_groups = 0;
    const char * first_group_name = NULL;
    int first_group_index = -1;
    bool has_first_named = false;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    observed_pattern = cre2_pattern(re);
    capturing_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    has_first_named = cre2_named_groups_iter_next(iter, &first_group_name, &first_group_index);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)capturing_groups + (long)first_group_index + (has_first_named ? 1L : 0L) + (observed_pattern ? 0L : 0L);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}