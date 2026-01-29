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
//<ID> 295
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(HELLO)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "HELLO-42 trailing data";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char aux_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int log_errors_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int err_code = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(aux_buf, 0, sizeof(aux_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Create compiled regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Operate / Validate
    err_code = cre2_error_code(re);
    aux_buf[0] = (char)((unsigned char)aux_buf[0] + (unsigned)((int)enc + log_errors_flag + (err_code & 0xFF)));
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((int)aux_buf[0] & 0x7));

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}