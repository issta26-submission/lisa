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
//<ID> 793
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations and Initialize
    const char pattern[] = "foo([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text[] = "prefix foo1234 suffix";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int rc_set_add = 0;
    int rc_set_compile = 0;
    int match_buf[8];
    const size_t match_len = 8;
    size_t nmatched = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int literal_flag = 0;
    int computed_check = 0;

    // step 2: Configure options
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Operate - compile regexp and build set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    rc_set_add = cre2_set_add_simple(set, pattern);
    rc_set_compile = cre2_set_compile(set);

    // step 4: Operate - match against text
    memset(match_buf, -1, sizeof(match_buf));
    nmatched = cre2_set_match(set, text, text_len, match_buf, match_len);

    // step 5: Validate using option queries and match results
    enc = cre2_opt_encoding(opt);
    literal_flag = cre2_opt_literal(opt);
    computed_check = (int)nmatched + rc_set_add + rc_set_compile + (int)enc + literal_flag + (int)pattern[0] + match_buf[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}