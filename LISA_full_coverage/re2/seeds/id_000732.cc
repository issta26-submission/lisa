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
//<ID> 732
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
    cre2_string_t match;
    int match_array[8];
    const char * ng_name = NULL;
    int ng_index = 0;
    int rc_add = 0;
    int rc_compile = 0;
    int rc_match = 0;
    size_t set_matches = 0;
    int rc_num_groups = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    bool ng_has_next = false;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    memset(match_array, 0, sizeof(match_array));

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    match.data = NULL;
    match.length = 0;
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 3);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_array, (size_t)(sizeof(match_array)/sizeof(match_array[0])));
    rc_num_groups = cre2_num_capturing_groups(re);
    enc = cre2_opt_encoding(opt);
    ng_has_next = cre2_named_groups_iter_next(iter, &ng_name, &ng_index);

    // step 5: Validate
    computed_check = rc_match + (int)set_matches + rc_num_groups + (int)enc + rc_add + rc_compile + ng_index + (ng_name ? (int)ng_name[0] : 0) + match.length + (int)ng_has_next + match_array[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}