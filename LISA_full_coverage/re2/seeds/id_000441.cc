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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(?P<second>bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|bar|baz";
    const char text_buf[] = "zzfooBazbarfoo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t text_sp;
    cre2_string_t match;
    int match_indices[8];
    int set_add_res = 0;
    int one_line_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int group_count = 0;
    size_t set_matches = 0;
    const char *found_name = NULL;
    int found_index = 0;
    int iter_next_res = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&text_sp, 0, sizeof(text_sp));
    memset(&match, 0, sizeof(match));
    memset(match_indices, 0, sizeof(match_indices));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    set_add_res = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    iter_next_res = (int)cre2_named_groups_iter_next(iter, &found_name, &found_index);
    one_line_flag = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);
    group_count = cre2_num_capturing_groups(re);
    set_matches = cre2_set_match(set, text_buf, text_len, match_indices, (size_t)(sizeof(match_indices) / sizeof(match_indices[0])));

    // step 5: Validate
    computed_check = set_add_res + one_line_flag + (int)enc + group_count + (int)set_matches + iter_next_res + found_index + (int)(found_name != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}