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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix foo123 suffix";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    const char * compiled_pat = NULL;
    int one_line_flag = 0;
    int match_res = 0;
    int err_code = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    match.data = text_buf;
    match.length = sample_text_len;
    opt = cre2_opt_new();

    // step 3: Configure / Create
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate / Validate
    match_res = cre2_match(re, text_buf, sample_text_len, 0, sample_text_len, (cre2_anchor_t)0, &match, 1);
    compiled_pat = re ? cre2_pattern(re) : NULL;
    err_code = re ? cre2_error_code(re) : 0;
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((one_line_flag & 0x1) + (match_res & 0x7) + (err_code & 0x3)));
    (void)compiled_pat;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}