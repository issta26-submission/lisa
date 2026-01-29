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
//<ID> 1676
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    const char *compiled_pat = NULL;
    int num_groups = 0;
    cre2_named_groups_iter_t *iter = NULL;
    const char *found_name = NULL;
    int found_index = -1;
    bool iter_rc = false;
    long checksum = 0;
    char scratch[64];

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);
    memset(scratch, 0, sizeof(scratch));

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    iter_rc = cre2_named_groups_iter_next(iter, &found_name, &found_index);
    cre2_named_groups_iter_delete(iter);
    iter = NULL;

    // step 5: Validate
    checksum = (long)pattern_len + (long)num_groups + (long)(compiled_pat != NULL) + (long)iter_rc
               + (long)(found_name != NULL) + (long)found_index + (long)(opt != NULL) + (long)(re != NULL)
               + (long)scratch[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}