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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(Hello|Hi),\\s*(\\w+)!$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Hi, Alice!";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t single_match;
    int pm_res = 0;
    const char * compiled_pattern = NULL;
    int64_t max_mem_val = 0;
    int num_caps = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t) (16 * 1024 * 1024));
    memset(match_buf, 0, sizeof(match_buf));

    // step 3: Core operations (compile regex)
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pattern = cre2_pattern(re);

    // step 4: Operate (prepare subject and perform a partial match using compiled regex)
    subject.data = text_buf;
    subject.length = text_len;
    single_match.data = match_buf;
    single_match.length = 0;
    pm_res = cre2_partial_match_re(re, &subject, &single_match, 3);

    // step 5: Validate / compute lightweight checksum (use cre2_opt_max_mem and cre2_pattern outputs)
    max_mem_val = cre2_opt_max_mem(opt);
    num_caps = cre2_num_capturing_groups(re);
    computed_check = (long)pm_res + (long)max_mem_val + (long)num_caps + (long)single_match.length + (long)pattern_len + (long)subject.length + (long)(compiled_pattern[0]);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}