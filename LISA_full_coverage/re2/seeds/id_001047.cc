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
//<ID> 1047
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
    char match0_buf[64];
    char match1_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t subject;
    cre2_string_t matches[2];
    int set_match_arr[8];
    const size_t set_match_len = sizeof(set_match_arr) / sizeof(set_match_arr[0]);
    size_t set_matches = 0;
    int partial_res = 0;
    int log_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup (options, buffers and structures)
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_flag = cre2_opt_log_errors(opt);
    enc = cre2_opt_encoding(opt);
    memset(match0_buf, 0, sizeof(match0_buf));
    memset(match1_buf, 0, sizeof(match1_buf));
    memset(set_match_arr, 0, sizeof(set_match_arr));

    // step 3: Core operations - compile regex and prepare set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Prepare cre2_string_t inputs for partial match
    subject.data = text_buf;
    subject.length = text_len;
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = match1_buf;
    matches[1].length = 0;

    // step 5: Operate - run partial match and set match
    partial_res = cre2_partial_match(pattern, &subject, matches, 2);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, set_match_arr, set_match_len);

    // step 6: Validate / Cleanup
    computed_check = (long)partial_res + (long)set_matches + (long)matches[0].length + (long)matches[1].length + (long)pattern_len + (long)subject.length + (long)log_flag + (long)enc;
    (void)computed_check;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}