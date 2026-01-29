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
//<ID> 2108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "Hello (RE2)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Hello RE2 and more";
    int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input = { text_buf, text_len };
    cre2_string_t matches[3];
    int log_errors_flag = 0;
    int partial_result = 0;
    const char *pattern_from_re = NULL;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    pattern_from_re = cre2_pattern(re);
    memset(matches, 0, sizeof(matches));
    partial_result = cre2_partial_match(pattern_from_re, &input, matches, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)log_errors_flag + (long)partial_result + (long)input.length + (long)matches[0].length + (long)matches[1].length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}