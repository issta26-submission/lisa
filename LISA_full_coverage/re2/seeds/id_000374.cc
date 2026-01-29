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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarbazyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_sp;
    cre2_string_t matches[4];
    int easy_res = 0;
    int partial_res = 0;
    int nmatch = 4;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &matches[0], 1);
    partial_res = cre2_partial_match_re(re, &text_sp, matches, nmatch);

    // step 5: Validate
    computed_check = easy_res + partial_res + matches[0].length + matches[1].length + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}