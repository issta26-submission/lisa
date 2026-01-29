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
//<ID> 826
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
    char text_buf[] = "xxabcyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[2];
    cre2_string_t input_str;
    cre2_string_t out_match;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int res_match = 0;
    int res_partial = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(match_arr, 0, sizeof(match_arr));
    res_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 2);
    memset(&input_str, 0, sizeof(input_str));
    input_str.data = text_buf + 2;
    input_str.length = 3; // "abc"
    memset(&out_match, 0, sizeof(out_match));
    res_partial = cre2_partial_match(pattern, &input_str, &out_match, 1);

    // step 4: Validate
    computed_check = (long)enc + (long)res_match + (long)res_partial + (long)match_arr[0].length + (long)out_match.length + (long)pattern[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}