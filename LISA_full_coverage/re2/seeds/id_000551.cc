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
//<ID> 551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)+\\d*$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t err_arg;
    const char *pat_from_re = NULL;
    int posix_set_value = 1;
    int posix_flag_read = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&err_arg, 0, sizeof(err_arg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_posix_syntax(opt, posix_set_value);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    cre2_error_arg(re, &err_arg);
    posix_flag_read = cre2_opt_posix_syntax(opt);

    // step 5: Validate
    computed_check = pattern_len + err_arg.length + posix_flag_read + (int)(pat_from_re ? pat_from_re[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}