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
//<ID> 827
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c?)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabczz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_str;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int match_res_re = 0;
    int match_res_partial = 0;
    int startpos = 0;
    int endpos = text_len;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    memset(&text_str, 0, sizeof(text_str));
    text_str.data = text_buf;
    text_str.length = text_len;
    match_res_re = cre2_match(re, text_buf, text_len, startpos, endpos, CRE2_UNANCHORED, matches, 4);
    match_res_partial = cre2_partial_match(pattern, &text_str, matches, 4);

    // step 4: Validate
    computed_check = (long)enc + (long)match_res_re + (long)match_res_partial + (long)matches[0].length + (long)matches[1].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}