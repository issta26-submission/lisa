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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)c(de)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hello|cre2";
    const size_t set_pattern_len = (size_t)(sizeof(set_pattern) - 1);
    const char set_text[] = "xxhelloyy";
    const size_t set_text_len = (size_t)(sizeof(set_text) - 1);
    const char easy_text[] = "abcde and more";
    const int easy_text_len = (int)(sizeof(easy_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t easy_matches[3];
    int set_matches[4];
    size_t set_match_count = 0;
    int easy_res = 0;
    int num_groups = 0;
    int compiled_ok = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(set_matches, 0, sizeof(set_matches));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);
    cre2_set_add_simple(set, set_pattern);
    compiled_ok = cre2_set_compile(set);

    // step 4: Operate
    set_match_count = cre2_set_match(set, set_text, set_text_len, set_matches, sizeof(set_matches) / sizeof(set_matches[0]));
    easy_res = cre2_easy_match(pattern, pattern_len, easy_text, easy_text_len, easy_matches, 3);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate
    computed_check = (int)set_match_count + easy_res + num_groups + set_matches[0] + easy_matches[0].length + compiled_ok + pattern_len + (int)set_text_len + easy_text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}