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
//<ID> 2071
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(?P<id>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int num_groups = 0;
    const char *iter_name = NULL;
    int iter_index = -1;
    bool iter_has_next = false;
    int found_index_by_name = -999;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup (create options)
    opt = cre2_opt_new();

    // step 3: Configure (compile regexp with options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect capturing groups and named groups)
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    iter_has_next = cre2_named_groups_iter_next(iter, &iter_name, &iter_index);
    found_index_by_name = cre2_find_named_capturing_groups(re, iter_name);

    // step 5: Validate (combine values so data flows through)
    long checksum = 0;
    checksum += (long)pattern_len;
    checksum += (long)num_groups;
    checksum += (long)iter_index;
    checksum += (long)found_index_by_name;
    checksum += (long)iter_has_next;
    checksum += (long)(iter_name != NULL);
    checksum += (long)(re != NULL);
    checksum += (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (delete iterator, regexp and options)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}