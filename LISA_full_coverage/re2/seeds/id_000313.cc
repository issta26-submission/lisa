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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "helloworld";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * compiled_pat = NULL;
    const char * errstr = NULL;
    int one_line_flag_set = 1;
    int case_sensitive_flag_set = 1;
    int first_char = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, one_line_flag_set);
    cre2_opt_set_case_sensitive(opt, case_sensitive_flag_set);

    // step 4: Core operations / Validate
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    errstr = cre2_error_string(re);
    iter = cre2_named_groups_iter_new(re);
    first_char = compiled_pat ? (int)(unsigned char)compiled_pat[0] : 0;
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((first_char & 0x7) + (errstr ? (int)(unsigned char)errstr[0] & 0x3 : 0) + (one_line_flag_set & 0x1) + (case_sensitive_flag_set & 0x1)));

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}