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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(abc)(def)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "abcdef";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * err_str = NULL;
    int one_line_flag = 1;
    int case_sensitive_flag = 1;
    unsigned char adj = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);

    // step 3: Configure / Create
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, one_line_flag);
    cre2_opt_set_case_sensitive(opt, case_sensitive_flag);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate / Validate
    err_str = cre2_error_string(re);
    adj = (unsigned char)(err_str ? (unsigned char)err_str[0] : 0);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned char)((adj & 0x7) + (one_line_flag & 0x1) + (case_sensitive_flag & 0x1)));
    iter = cre2_named_groups_iter_new(re);

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}