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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>abc)(def)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "abc def";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * err_str = NULL;
    const char * compiled_pat = NULL;
    int name_index = -1;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);

    // step 3: Configure / Create
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate / Validate
    compiled_pat = cre2_pattern(re);
    err_str = cre2_error_string(re);
    name_index = cre2_find_named_capturing_groups(re, "name");
    iter = cre2_named_groups_iter_new(re);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned char)((compiled_pat && compiled_pat[0]) ? (compiled_pat[0] & 0x7) : 0));
    (void)err_str;
    (void)name_index;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}