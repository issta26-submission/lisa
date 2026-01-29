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
//<ID> 527
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
    const char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t input[1];
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t err_arg;
    int nmatch = 3;
    int partial_ret = 0;
    int global_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(input, 0, sizeof(input));
    memset(matches, 0, sizeof(matches));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&err_arg, 0, sizeof(err_arg));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    input[0].data = text_buf;
    input[0].length = text_len;
    partial_ret = cre2_partial_match_re(re, input, matches, nmatch);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    global_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    cre2_error_arg(re, &err_arg);

    // step 5: Validate
    computed_check = partial_ret + global_ret + matches[0].length + matches[1].length + err_arg.length + pattern_len + text_and_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}