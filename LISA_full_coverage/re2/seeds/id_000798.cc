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
//<ID> 798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "ab(c)?d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx abcd yy ab d zz";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int match_buf[8];
    size_t match_len = (size_t)(sizeof(match_buf) / sizeof(match_buf[0]));
    size_t rc_match = 0;
    int rc_set_add = 0;
    int rc_set_compile = 0;
    int computed_check = 0;
    memset(match_buf, 0, sizeof(match_buf));

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 1);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_set_add = cre2_set_add_simple(set, pattern);
    rc_set_compile = cre2_set_compile(set);
    rc_match = cre2_set_match(set, text_buf, text_len, match_buf, match_len);

    // step 4: Validate
    computed_check = (int)rc_match + rc_set_add + rc_set_compile + cre2_opt_literal(opt) + (int)cre2_opt_encoding(opt) + match_buf[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}