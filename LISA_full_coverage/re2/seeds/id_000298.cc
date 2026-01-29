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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a+b?c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "literal a+b?c inside text";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int log_before = 0;
    int log_after = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int enc_int = 0;
    int result_acc = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    log_before = cre2_opt_log_errors(opt);

    // step 4: Create compiled regexp with configured options
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Operate / Validate
    enc = cre2_opt_encoding(opt);
    enc_int = (int)enc;
    log_after = cre2_opt_log_errors(opt);
    result_acc = pattern_len + sample_text_len + enc_int + (log_before ^ log_after);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)(result_acc & 0x7));
    (void)result_acc;
    (void)enc_int;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}