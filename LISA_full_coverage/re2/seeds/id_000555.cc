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
//<ID> 555
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t err_arg;
    const char *pattern_from_re = NULL;
    int posix_flag = 1;
    int computed_check = 0;

    // step 2: Setup
    memset(&err_arg, 0, sizeof(err_arg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, posix_flag);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);

    // step 5: Validate
    cre2_error_arg(re, &err_arg);
    computed_check = pattern_len + err_arg.length + (int)(pattern_from_re == pattern);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}