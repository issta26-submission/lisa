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
//<ID> 1474
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
    char text_buf[] = "xxx foo bar yyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match;
    int log_errors_flag = 0;
    int easy_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)easy_ret + (long)match.length + (long)log_errors_flag
               + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    if (iter) { /* no control flow other than cleanup */ cre2_named_groups_iter_delete(iter); }
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}