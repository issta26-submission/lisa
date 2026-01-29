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
//<ID> 1353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    const char * pattern_from_re = NULL;
    int opt_cs = 0;
    int opt_le = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 0);

    // step 4: Core operations
    opt_cs = cre2_opt_case_sensitive(opt);
    opt_le = cre2_opt_log_errors(opt);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    pattern_from_re = cre2_pattern(re);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)opt_cs + (long)opt_le
                     + (long)(re != NULL) + (long)(iter != NULL) + (long)(set != NULL)
                     + (long)(pattern_from_re != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}