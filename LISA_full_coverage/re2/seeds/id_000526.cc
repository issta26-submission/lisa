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
//<ID> 526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabcxxabcyxx";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t match_arr[4];
    int nmatch = 4;
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    cre2_string_t err_arg;
    int partial_ret = 0;
    int replace_count = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(&err_arg, 0, sizeof(err_arg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    subject.data = text_buf;
    subject.length = text_len;
    partial_ret = cre2_partial_match_re(re, &subject, match_arr, nmatch);
    cre2_error_arg(re, &err_arg);
    rewrite.data = "R";
    rewrite.length = 1;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    replace_count = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = partial_ret + replace_count + (int)err_arg.length + match_arr[0].length + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}