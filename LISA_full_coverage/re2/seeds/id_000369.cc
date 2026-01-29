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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(bc)?d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxabcdyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_sp;
    cre2_string_t matches[4];
    cre2_string_t partial_matches[2];
    int never_nl_flag = 0;
    int partial_res = 0;
    int match_res = 0;
    const char * compiled_pat = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_never_nl(opt, 1);
    never_nl_flag = cre2_opt_never_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    partial_res = cre2_partial_match(pattern, &text_sp, partial_matches, 2);
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);

    // step 5: Validate
    compiled_pat = cre2_pattern(re);
    computed_check = partial_res + match_res + never_nl_flag + matches[0].length + partial_matches[0].length + pattern_len + text_len;
    (void)computed_check;
    (void)compiled_pat;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}