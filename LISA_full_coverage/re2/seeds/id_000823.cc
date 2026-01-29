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
//<ID> 823
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
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t text_string;
    cre2_string_t match_arr[3];
    cre2_string_t partial_match_arr[3];
    int match_res = 0;
    int partial_res = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    memset(match_arr, 0, sizeof(match_arr));
    memset(partial_match_arr, 0, sizeof(partial_match_arr));
    text_string.data = text_buf;
    text_string.length = text_len;
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 2);
    partial_res = cre2_partial_match(pattern, &text_string, partial_match_arr, 2);

    // step 4: Validate
    num_groups = cre2_num_capturing_groups(re);
    computed_check = (long)enc + (long)match_res + (long)partial_res + (long)num_groups + (long)match_arr[0].length + (long)partial_match_arr[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}