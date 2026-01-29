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
//<ID> 737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx ab yy a b ab";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match;
    int rc_match = 0;
    int rc_add = 0;
    int rc_compile = 0;
    size_t set_matches = 0;
    int match_indices[4];
    int num_groups = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    match.data = NULL;
    match.length = 0;
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 1);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, (size_t)4);
    num_groups = cre2_num_capturing_groups(re);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    computed_check = rc_match + (int)set_matches + num_groups + (int)enc + match.length + match_indices[0] + rc_add + rc_compile;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}