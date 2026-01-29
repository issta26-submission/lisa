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
//<ID> 521
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
    const char text_buf1[] = "xxfoobarbaz foo";
    const int text_len1 = (int)(sizeof(text_buf1) - 1);
    const char text_buf2[] = "foo bar foo";
    const int text_len2 = (int)(sizeof(text_buf2) - 1);
    const char rewrite_buf[] = "X$1";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t input1;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t error_arg;
    int partial_ret = 0;
    int replace_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(&input1, 0, sizeof(input1));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&error_arg, 0, sizeof(error_arg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    input1.data = text_buf1;
    input1.length = text_len1;
    partial_ret = cre2_partial_match_re(re, &input1, match_arr, 3);
    cre2_error_arg(re, &error_arg);
    text_and_target.data = text_buf2;
    text_and_target.length = text_len2;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = partial_ret + replace_ret + match_arr[0].length + match_arr[1].length + error_arg.length + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}