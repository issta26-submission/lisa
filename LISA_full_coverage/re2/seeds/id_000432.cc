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
//<ID> 432
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
    const char text_buf[] = "foo bar foobar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text_sp;
    cre2_string_t rewrite_sp;
    int add_res = 0;
    int replace_res = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&text_sp, 0, sizeof(text_sp));
    memset(&rewrite_sp, 0, sizeof(rewrite_sp));
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024); // limit to 1MB
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    add_res = cre2_set_add_simple(set, set_pattern);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    text_sp.data = text_buf;
    text_sp.length = text_len;
    rewrite_sp.data = rewrite_buf;
    rewrite_sp.length = rewrite_len;
    replace_res = cre2_global_replace_re(re, &text_sp, &rewrite_sp);

    // step 5: Validate
    computed_check = replace_res + add_res + text_sp.length + rewrite_sp.length + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}