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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>abc)(def)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "abcDEFghi";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * iter_name = NULL;
    int iter_index = -1;
    const char * err_str = NULL;
    bool iter_got = false;
    int accum = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure / Create
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate / Validate
    iter_got = cre2_named_groups_iter_next(iter, &iter_name, &iter_index);
    err_str = cre2_error_string(re);
    accum = (int)((err_str ? (unsigned char)err_str[0] : 0) + (iter_index & 0xff) + (iter_got ? 1 : 0));
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((accum & 0x7)));

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}