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
//<ID> 2070
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int num_groups = 0;
    int named_idx = -1;
    const char *iter_name = NULL;
    int iter_index = -1;
    char scratch[32];
    long checksum = 0;

    // step 2: Setup (create options and initialize scratch)
    opt = cre2_opt_new();
    memset(scratch, 0, sizeof(scratch));

    // step 3: Core operations (compile regexp, query captures and named groups)
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    named_idx = cre2_find_named_capturing_groups(re, "name");
    cre2_named_groups_iter_next(iter, &iter_name, &iter_index);

    // step 4: Validate (combine results so data flows through)
    checksum = (long)pattern_len + (long)num_groups + (long)named_idx + (long)iter_index + (long)(iter_name != NULL) + (long)(re != NULL) + (long)(opt != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 5: Cleanup (delete iterator, regexp and options)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}