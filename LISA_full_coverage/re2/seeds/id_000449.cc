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
//<ID> 449
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?(?P<name>baz)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hello|foo|bar|baz";
    const char text_buf[] = "xxfoo barbar foobaz hello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *found_name = NULL;
    int found_index = 0;
    int matches[8];
    size_t match_count = 0;
    int compiled_flag = 0;
    int opt_oneline_val = 0;
    cre2_encoding_t enc_val = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    re = cre2_new(pattern, pattern_len, opt);
    compiled_flag = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, matches, (size_t)(sizeof(matches) / sizeof(matches[0])));

    // step 5: Validate
    opt_oneline_val = cre2_opt_one_line(opt);
    enc_val = cre2_opt_encoding(opt);
    computed_check = (int)match_count + compiled_flag + opt_oneline_val + (int)enc_val + cre2_num_capturing_groups(re) + (found_name ? (int)found_index : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}