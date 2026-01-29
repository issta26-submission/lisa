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
//<ID> 439
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
    const char set_pattern[] = "foo|bar|baz";
    const char text_buf[] = "xxfoo barbar foobar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_count = 0;
    int compiled_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);
    cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate
    replace_count = cre2_global_replace_re(re, &text_and_target, &rewrite);
    compiled_flag = (int)cre2_set_add_simple(set, "unused_pattern"); /* benign second add */

    // step 5: Validate
    computed_check = replace_count + pattern_len + text_and_target.length + rewrite.length + compiled_flag;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}