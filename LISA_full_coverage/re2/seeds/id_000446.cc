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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hello|foo|bar";
    const char text_buf[] = "xxfoohello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int one_line_flag = 0;
    int match_indices[4];
    size_t match_count = 0;
    const char *ng_name = NULL;
    int ng_index = -1;
    int add_result = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    one_line_flag = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_result = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, 4);
    /* benign second add to exercise API */
    cre2_set_add_simple(set, "unused_pattern");

    // step 5: Validate
    computed_check = (int)match_count + one_line_flag + (int)enc + ng_index + add_result + match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}