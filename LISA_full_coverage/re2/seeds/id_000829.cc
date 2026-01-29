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
//<ID> 829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabczz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t match_arr[3];
    cre2_string_t pm_text;
    cre2_string_t pm_out[3];
    int match_res = 0;
    int pm_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    memset(match_arr, 0, sizeof(match_arr));
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 3);

    // step 4: Additional operations
    memset(pm_out, 0, sizeof(pm_out));
    memset(&pm_text, 0, sizeof(pm_text));
    pm_text.data = text_buf;
    pm_text.length = text_len;
    pm_res = cre2_partial_match(pattern, &pm_text, pm_out, 3);

    // step 5: Validate
    computed_check = (long)match_res + (long)pm_res + (long)enc + (long)(match_arr[0].length) + (long)(pm_out[0].length);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}