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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t errarg;
    int nonnull_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&errarg, 0, sizeof(errarg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    const char * pattern_from_re = cre2_pattern(re);
    cre2_error_arg(re, &errarg);
    nonnull_flag = (pattern_from_re != NULL);

    // step 5: Validate
    computed_check = pattern_len + errarg.length + nonnull_flag;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}