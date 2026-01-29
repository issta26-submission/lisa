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
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc([0-9]+)def";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "zzz abc123def yyy";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pat = NULL;
    int log_errors_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int err_code = 0;
    int adjust = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 3: Configure / Create
    log_errors_flag = cre2_opt_log_errors(opt);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    compiled_pat = re ? cre2_pattern(re) : NULL;
    err_code = re ? cre2_error_code(re) : 0;
    adjust = (int)enc + log_errors_flag + (err_code & 0x7) + (compiled_pat ? (int)compiled_pat[0] & 0xF : 0);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)adjust);

    // step 5: Validate (simple in-place validation via observed values)
    (void)compiled_pat;
    (void)err_code;
    (void)log_errors_flag;
    (void)enc;
    (void)adjust;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}