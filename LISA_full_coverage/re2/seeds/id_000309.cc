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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char sample_text[] = "sample";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[32];
    cre2_options_t * opt = NULL;
    int one_line_flag = 0;
    int wb_flag_set = 1;
    int perl_flag_set = 1;
    int wb_flag_read = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, perl_flag_set);
    cre2_opt_set_word_boundary(opt, wb_flag_set);

    // step 4: Operate / Validate
    one_line_flag = cre2_opt_one_line(opt);
    wb_flag_read = cre2_opt_word_boundary(opt);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned char)((one_line_flag & 0x1) + (wb_flag_read & 0x1) + (perl_flag_set & 0x1)));

    // step 5: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}