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
//<ID> 528
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    const char text_buf[] = "prefix foo middle foo suffix";
    const char rewrite_buf[] = "REPL";
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t text_in[1];
    cre2_string_t match_arr[3];
    cre2_string_t error_arg;
    int nmatch = 3;
    int ret_partial = 0;
    int ret_replace = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(text_in, 0, sizeof(text_in));
    memset(&error_arg, 0, sizeof(error_arg));
    opt = cre2_opt_new();

    // step 3: Configure / Initialize strings and regexp
    cre2_opt_set_case_sensitive(opt, 1);
    text_in[0].data = text_buf;
    text_in[0].length = (int)(sizeof(text_buf) - 1);
    text_and_target.data = text_buf;
    text_and_target.length = text_in[0].length;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate - partial match, get error arg, then perform global replace
    ret_partial = cre2_partial_match_re(re, text_in, match_arr, nmatch);
    cre2_error_arg(re, &error_arg);
    ret_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate (aggregate results into a sink variable)
    computed_check = ret_partial + ret_replace + match_arr[0].length + match_arr[1].length + error_arg.length + text_and_target.length + rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}