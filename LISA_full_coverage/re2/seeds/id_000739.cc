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
//<ID> 739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t matches[8];
    const int MAX_NMATCH = 8;
    int rc_match = 0;
    size_t set_matches = 0;
    int match_indices[16];
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    iter = cre2_named_groups_iter_new(re);
    num_groups = cre2_num_capturing_groups(re);

    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    matches[3].data = NULL; matches[3].length = 0;
    matches[4].data = NULL; matches[4].length = 0;
    matches[5].data = NULL; matches[5].length = 0;
    matches[6].data = NULL; matches[6].length = 0;
    matches[7].data = NULL; matches[7].length = 0;

    memset(match_indices, 0, sizeof(match_indices));
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, MAX_NMATCH);

    // step 4: Validate
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, sizeof(match_indices) / sizeof(match_indices[0]));
    enc = cre2_opt_encoding(opt);
    computed_check = rc_match + num_groups + (int)enc + (int)set_matches + matches[0].length + matches[1].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}