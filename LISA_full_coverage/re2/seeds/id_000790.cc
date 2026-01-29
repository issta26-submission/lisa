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
//<ID> 790
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "foo|bar|(baz)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx baz yy foo zz";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int matches[4];
    size_t match_count = 0;
    cre2_encoding_t encoding_obtained = CRE2_UNKNOWN;
    int literal_flag = 0;
    int rc_add = 0;
    int rc_compile = 0;
    int computed_check = 0;

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    encoding_obtained = cre2_opt_encoding(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    memset(matches, 0, sizeof(matches));
    match_count = cre2_set_match(set, text_buf, text_len, matches, (size_t)4);

    // step 4: Validate
    computed_check = (int)match_count + matches[0] + pattern_len + (int)encoding_obtained + literal_flag + rc_add + rc_compile + (int)((re != NULL) ? 1 : 0);
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}