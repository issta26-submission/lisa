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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\btest\\b(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "test123 other TEST456";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pat = NULL;
    int one_line_flag = 0;
    int wb_flag = 1;
    int perl_classes_flag = 1;
    int err_code = 0;
    int num_caps = 0;
    int cp0 = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, wb_flag);
    cre2_opt_set_perl_classes(opt, perl_classes_flag);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Configure / Create
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate / Validate
    compiled_pat = cre2_pattern(re);
    err_code = cre2_error_code(re);
    num_caps = cre2_num_capturing_groups(re);
    cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((cp0 & 0xF) + (one_line_flag & 0x1) + (num_caps & 0x3)));

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}