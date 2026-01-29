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
//<ID> 420
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
    const char set_pattern[] = "hello|foo";
    const char text_buf[] = "xxfoofoobarzzhello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_sp;
    int add_res = 0;
    int compile_res = 0;
    int full_res = 0;
    const char *pat_back = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);
    full_res = cre2_full_match(cre2_pattern(re), &text_sp, matches, 3);
    pat_back = cre2_pattern(re);

    // step 5: Validate
    computed_check = add_res + compile_res + full_res + matches[0].length + (int)text_sp.length + pattern_len + (int)(pat_back ? 0 : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}