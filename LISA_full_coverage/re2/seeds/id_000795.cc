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
//<ID> 795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int match_buf[4];
    size_t nmatch = 0;
    int rc_add = 0;
    int rc_compile = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int literal_flag = 0;
    int computed_check = 0;

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    memset(match_buf, 0, sizeof(match_buf));
    nmatch = cre2_set_match(set, text_buf, (size_t)text_len, match_buf, (size_t)(sizeof(match_buf)/sizeof(match_buf[0])));

    // step 4: Validate
    computed_check = pattern_len + (int)enc + literal_flag + rc_add + rc_compile + (int)nmatch + match_buf[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}