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
//<ID> 2079
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)-(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char lookup_name[] = "name";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int num_groups = 0;
    int found_index = -1;
    long checksum = 0;

    // step 2: Setup (initialize options)
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Configure (build a regexp from the pattern using options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (inspect regexp: count capturing groups, create named-group iterator, find named group)
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    found_index = cre2_find_named_capturing_groups(re, lookup_name);

    // step 5: Validate and Cleanup (combine results and release resources)
    checksum = (long)pattern_len + (long)num_groups + (long)found_index + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}