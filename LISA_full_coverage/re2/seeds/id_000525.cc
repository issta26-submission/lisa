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
//<ID> 525
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(?:bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    const char text_buf[] = "xxfoobarbaz foo bar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t input_arr[1];
    cre2_string_t matches[4];
    cre2_string_t rewrite;
    const char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_string_t errarg;
    int partial_ret = 0;
    int replace_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    input_arr[0].data = text_buf;
    input_arr[0].length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    errarg.data = NULL;
    errarg.length = 0;

    // step 4: Operate
    partial_ret = cre2_partial_match_re(re, (const cre2_string_t *)input_arr, matches, 4);
    cre2_error_arg(re, &errarg);
    replace_ret = cre2_global_replace_re(re, &input_arr[0], &rewrite);

    // step 5: Validate
    computed_check = partial_ret + replace_ret + matches[0].length + matches[1].length + errarg.length + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}