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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\bfoo(\\d+)\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix foo123 suffix";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * compiled_pat = NULL;
    int one_line_flag = 0;
    int num_caps = 0;
    int prog_size = 0;
    int cp0 = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Configure / Create
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate / Validate
    compiled_pat = cre2_pattern(re);
    num_caps = cre2_num_capturing_groups(re);
    prog_size = cre2_program_size(re);
    cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((cp0 & 0xF) + (num_caps & 0x7) + (one_line_flag & 0x1) + (prog_size & 0x3)));

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}