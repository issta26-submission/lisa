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
//<ID> 733
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx helloworld yy hello zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[4];
    int set_match_idxs[4];
    int rc_match = 0;
    size_t rc_set_matches = 0;
    int num_caps = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    memset(matches, 0, sizeof(matches));
    memset(set_match_idxs, 0xff, sizeof(set_match_idxs)); // initialize to -1
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    num_caps = cre2_num_capturing_groups(re);
    rc_set_matches = cre2_set_match(set, text_buf, (size_t)text_len, set_match_idxs, 4);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    computed_check = rc_match + (int)rc_set_matches + num_caps + (int)enc + matches[0].length + set_match_idxs[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}