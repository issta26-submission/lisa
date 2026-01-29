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
//<ID> 730
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
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[3];
    int set_match_indices[4];
    int rc_match = 0;
    size_t rc_set_match = 0;
    int num_groups = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    (void)cre2_set_add_simple(set, pattern);
    (void)cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Initialize match holders
    memset(matches, 0, sizeof(matches));
    memset(set_match_indices, 0, sizeof(set_match_indices));

    // step 4: Operate - perform matching operations
    num_groups = cre2_num_capturing_groups(re);
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    rc_set_match = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, 4);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    computed_check = rc_match + (int)rc_set_match + num_groups + (int)enc + matches[0].length + set_match_indices[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}