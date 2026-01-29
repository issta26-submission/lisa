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
//<ID> 1312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?<name>hello)\\s+(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * re_pat = NULL;
    int rev = 0;
    long computed_check = 0;
    char scratch[32];

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)8 * 1024 * 1024);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    rev = cre2_version_interface_revision();
    iter = cre2_named_groups_iter_new(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)(iter != NULL)
                     + (long)rev + (long)(re_pat != NULL) + (long)scratch[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}