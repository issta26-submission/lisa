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
//<ID> 1317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>hello)\\s+(world)(\\d{2})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * re_pat = NULL;
    const char * group_name = NULL;
    int group_index = -1;
    int revision = 0;
    long computed_check = 0;
    char scratch[64];

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)8 * 1024 * 1024);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    revision = cre2_version_interface_revision();
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &group_name, &group_index);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)(re_pat != NULL)
                     + (long)revision + (long)(group_name != NULL) + (long)group_index
                     + (long)(opt != NULL) + (long)sizeof(scratch);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}