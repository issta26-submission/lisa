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
//<ID> 1049
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123 bob456 carol789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[128];
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t subject;
    cre2_string_t single_match;
    int pm_res = 0;
    size_t set_matches = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int log_err_flag = 0;
    long computed_check = 0;

    // step 2: Setup (initialize options and buffers)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    memset(match_buf, 0, sizeof(match_buf));
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Prepare cre2_string_t structures and compile regex/set
    subject.data = text_buf;
    subject.length = text_len;
    single_match.data = match_buf;
    single_match.length = 0;
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate - perform partial match and set match
    pm_res = cre2_partial_match(pattern, &subject, &single_match, 2);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);

    // step 5: Validate / compute lightweight checksum using option queries
    enc = cre2_opt_encoding(opt);
    log_err_flag = cre2_opt_log_errors(opt);
    computed_check = (long)pm_res + (long)set_matches + (long)single_match.length + (long)pattern_len + (long)subject.length + (long)enc + (long)log_err_flag;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}