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
//<ID> 2074
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
    int name_index = -1;
    const char *pat_from_re = NULL;
    long checksum = 0;

    // step 2: Setup (initialize options)
    opt = cre2_opt_new();

    // step 3: Core operations (compile regexp and inspect captures)
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    name_index = cre2_find_named_capturing_groups(re, group_name);
    pat_from_re = cre2_pattern(re);

    // step 4: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)num_groups + (long)name_index + (long)(re != NULL) + (long)(iter != NULL) + (long)(pat_from_re != NULL);
    (void)checksum;

    // step 5: Cleanup (delete iterator, regexp and options)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}