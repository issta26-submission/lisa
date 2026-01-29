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
//<ID> 2077
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<id>[0-9]+)-(?P<name>[A-Za-z]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int num_groups = 0;
    int index_id = -1;
    int index_name = -1;
    const char *iter_name = NULL;
    int iter_index = -1;
    bool iter_got = false;
    long checksum = 0;

    // step 2: Setup (create options and compile regexp)
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure (inspect capturing groups and find named indices)
    num_groups = cre2_num_capturing_groups(re);
    index_id = cre2_find_named_capturing_groups(re, "id");
    index_name = cre2_find_named_capturing_groups(re, "name");
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate (attempt one iteration of named-groups iterator)
    iter_got = cre2_named_groups_iter_next(iter, &iter_name, &iter_index);

    // step 5: Validate (combine results so values flow through)
    checksum = (long)pattern_len + (long)num_groups + (long)index_id + (long)index_name + (long)(iter != NULL) + (long)(re != NULL) + (long)(iter_got ? 1 : 0) + (long)iter_index + (long)(iter_name != NULL);
    (void)checksum;

    // step 6: Cleanup (delete iterator, regexp and options)
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}