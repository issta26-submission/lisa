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
//<ID> 791
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab+c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text[] = "zz abbbc yy";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int match_indices[4] = { -1, -1, -1, -1 };
    size_t matched = 0;
    int literal_flag = 0;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int rc_set_add = 0;
    int rc_set_compile = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup / Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_set_add = cre2_set_add_simple(set, pattern);
    rc_set_compile = cre2_set_compile(set);
    matched = cre2_set_match(set, text, text_len, match_indices, 4);

    // step 4: Validate
    literal_flag = cre2_opt_literal(opt);
    encoding = cre2_opt_encoding(opt);
    num_groups = cre2_num_capturing_groups(re);
    computed_check = rc_set_add + rc_set_compile + (int)matched + literal_flag + (int)encoding + num_groups + match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}