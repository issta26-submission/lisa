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
//<ID> 522
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
    const char text_buf[] = "xxfoobarbaz foo bar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t captures[3];
    cre2_string_t error_arg;
    int nmatch = 3;
    int partial_ret = 0;
    int global_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(captures, 0, sizeof(captures));
    memset(&error_arg, 0, sizeof(error_arg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    text_str.data = text_buf;
    text_str.length = text_len;
    partial_ret = cre2_partial_match_re(re, &text_str, captures, nmatch);

    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;
    global_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    cre2_error_arg(re, &error_arg);

    // step 5: Validate
    computed_check = pattern_len + text_len + partial_ret + global_ret + captures[0].length + error_arg.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}