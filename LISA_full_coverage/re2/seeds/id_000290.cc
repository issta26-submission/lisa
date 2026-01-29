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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "LITERAL:.*\\d+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix LITERAL:12345 suffix";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    int match_res = 0;
    int nmatch = 2;
    int log_errors_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int err_code = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Create compiled regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Operate / Validate
    match_res = cre2_match(re, text_s.data, text_s.length, 0, text_s.length, (cre2_anchor_t)0, &match_s, nmatch);
    err_code = cre2_error_code(re);
    match_s.length = match_s.length + ((match_res & 0x7) + (int)enc);
    text_s.length = text_s.length - ((err_code & 0x3) + (log_errors_flag & 0x1));
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((match_s.length & 0xF) + (err_code & 0x7)));

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}