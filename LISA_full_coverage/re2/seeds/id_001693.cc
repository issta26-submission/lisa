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
//<ID> 1693
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foobarbaz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int errcode = 0;
    int rc_partial = 0;
    int rc_extract = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    cre2_string_t input_arr[1] = { { text_buf, text_len } };
    cre2_string_t matches[3] = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
    rc_partial = cre2_partial_match(pattern, input_arr, matches, 3);

    cre2_string_t text_str = { text_buf, text_len };
    cre2_string_t rewrite_str = { rewrite_buf, rewrite_len };
    memset(target_buf, 0, sizeof(target_buf));
    cre2_string_t target_str = { target_buf, 0 };
    rc_extract = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    errcode = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)rc_partial + (long)rc_extract + (long)errcode + (long)enc + (long)(opt != NULL) + (long)(re != NULL) + (long)target_str.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}