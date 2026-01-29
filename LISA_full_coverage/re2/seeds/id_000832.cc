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
//<ID> 832
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(?P<g>b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabczz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int case_flag = 1;
    int match_idx[4];
    size_t set_matches = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(match_idx, 0, sizeof(match_idx));
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_delete(iter);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_idx, (size_t)(sizeof(match_idx)/sizeof(match_idx[0])));

    // step 5: Validate
    computed_check = (long)enc + (long)case_flag + (long)set_matches + (long)match_idx[0] + (long)(pattern[0]);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}