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
//<ID> 303
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "Sample text for CRE2 options demonstration";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    cre2_options_t * opt = NULL;
    int one_line_before = 0;
    int one_line_after = 0;
    int wb_flag = 1;
    int perl_flag = 1;
    int accumulator = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();
    one_line_before = cre2_opt_one_line(opt);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, wb_flag);
    cre2_opt_set_perl_classes(opt, perl_flag);
    cre2_opt_set_one_line(opt, 1);
    one_line_after = cre2_opt_one_line(opt);

    // step 4: Operate / Validate
    accumulator = one_line_before + one_line_after + wb_flag + perl_flag + sample_text_len;
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)(accumulator & 0xFF));
    text_buf[sample_text_len] = (char)(accumulator & 0x7F);

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}