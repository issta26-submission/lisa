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
//<ID> 2072
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char group_name[] = "name";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int num_groups = 0;
    int found_index = -1;
    const char *pat_from_re = NULL;
    long checksum = 0;

    // step 2: Setup (create options)
    opt = cre2_opt_new();

    // step 3: Configure (compile regexp with options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect capturing groups and named groups)
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    found_index = cre2_find_named_capturing_groups(re, group_name);

    // step 5: Validate (pull pattern and combine values so data flows)
    pat_from_re = cre2_pattern(re);
    checksum = (long)pattern_len + (long)num_groups + (long)found_index + (long)(re != NULL) + (long)(iter != NULL) + (long)(pat_from_re != NULL);
    (void)checksum;

    // step 6: Cleanup (delete iterator, regexp and options)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}