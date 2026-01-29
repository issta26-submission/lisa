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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)|(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "alpha|beta|foo";
    const char text_buf[] = "xxfooxxbaralpha";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_sp;
    const char *ng_name = NULL;
    int ng_index = 0;
    int one_line_flag = 0;
    int encoding_val = 0;
    int set_add_res = 0;
    int match_res = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&text_sp, 0, sizeof(text_sp));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set_add_res = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    (void)cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    one_line_flag = cre2_opt_one_line(opt);
    encoding_val = (int)cre2_opt_encoding(opt);
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);

    // step 5: Validate
    computed_check = pattern_len + text_len + match_res + set_add_res + ng_index + one_line_flag + encoding_val + (int)(ng_name ? 1 : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}