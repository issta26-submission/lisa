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
//<ID> 2102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "User:(\\w+):([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "User:alice:42";
    int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input = { text_buf, text_len };
    cre2_string_t matches[3];
    int partial_result = 0;
    int log_errors_flag = 0;
    const char *compiled_pattern_ptr = NULL;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_log_errors(opt, 1);

    // step 3: Configure (compile regexp using options)
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern_ptr = cre2_pattern(re);

    // step 4: Operate (prepare match storage and perform partial match using raw pattern)
    memset(matches, 0, sizeof(matches));
    partial_result = cre2_partial_match(pattern, &input, matches, 3);

    // step 5: Validate (read option state and combine results so values flow)
    log_errors_flag = cre2_opt_log_errors(opt);
    checksum = (long)pattern_len + (long)partial_result + (long)log_errors_flag + (long)matches[0].length + (long)matches[1].length + (long)(re != NULL) + (long)(compiled_pattern_ptr != NULL) + (long)input.length;
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}