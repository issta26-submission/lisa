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
//<ID> 822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabczz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t match;
    cre2_string_t pm_match;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_anchor_t anchor = CRE2_UNANCHORED;
    int match_res = 0;
    int pm_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate - compile and run full match API
    re = cre2_new(pattern, pattern_len, opt);
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text_buf;
    text_str.length = text_len;
    memset(&match, 0, sizeof(match));
    match.data = NULL;
    match.length = 0;
    match_res = cre2_match(re, text_str.data, text_str.length, 0, text_str.length, anchor, &match, 1);

    // step 4: Operate - run partial match helper using the pattern and same text
    memset(&pm_match, 0, sizeof(pm_match));
    pm_match.data = NULL;
    pm_match.length = 0;
    pm_res = cre2_partial_match(pattern, &text_str, &pm_match, 1);

    // step 5: Validate
    computed_check = (long)enc + (long)match_res + (long)pm_res + (long)match.length + (long)pm_match.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}