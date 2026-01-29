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
//<ID> 821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab(c)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabcYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match[2];
    cre2_string_t partial_input[1];
    cre2_string_t partial_output[1];
    int match_res = 0;
    int partial_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    memset(match, 0, sizeof(match));
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match, 2);
    memset(partial_input, 0, sizeof(partial_input));
    partial_input[0].data = text_buf + 2;
    partial_input[0].length = 3;
    memset(partial_output, 0, sizeof(partial_output));
    partial_res = cre2_partial_match(pattern, partial_input, partial_output, 1);

    // step 4: Validate
    computed_check = (long)match_res + (long)partial_res + (long)match[0].length + (long)partial_output[0].length + (long)enc + (long)pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}