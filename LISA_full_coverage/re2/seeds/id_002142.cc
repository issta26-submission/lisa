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
//<ID> 2142
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
    char subject_buf[] = "User42 logged in";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_string_t subject = { subject_buf, subject_len };
    cre2_string_t match_arr[4];
    memset(match_arr, 0, sizeof(match_arr));
    cre2_options_t * opt = NULL;
    int easy_ret = 0;
    int partial_ret = 0;
    int literal_flag = 0;
    int case_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_word_boundary(opt, 1); // idempotent call to exercise API
    case_flag = cre2_opt_case_sensitive(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 3: Operate (perform easy match and partial match)
    easy_ret = cre2_easy_match(pattern, pattern_len, subject_buf, subject_len, &match_arr[0], 1);
    partial_ret = cre2_partial_match(pattern, &subject, match_arr, 3);

    // step 4: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)partial_ret + (long)match_arr[0].length + (long)match_arr[1].length + (long)match_arr[2].length + (long)literal_flag + (long)case_flag + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup (release resources)
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}