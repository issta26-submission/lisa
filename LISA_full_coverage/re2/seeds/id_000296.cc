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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "alpha(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "alpha123 beta456";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int log_before = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * compiled_pat = NULL;
    int err_code = 0;
    int first_char = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();

    // step 3: Configure
    log_before = cre2_opt_log_errors(opt);
    cre2_opt_set_literal(opt, 1);
    enc = cre2_opt_encoding(opt);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (int)enc + (log_before & 0x3));

    // step 4: Create regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Operate / Validate
    compiled_pat = cre2_pattern(re);
    err_code = cre2_error_code(re);
    first_char = compiled_pat ? (int)compiled_pat[0] : 0;
    text_buf[1] = (char)((unsigned char)text_buf[1] + ((first_char & 0xF) + (err_code & 0x7)));

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}