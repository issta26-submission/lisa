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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    const int nmatch = 3;
    int easy_ret = 0;
    int num_groups = 0;
    int err_code = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(matches, 0, sizeof(matches));
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, nmatch);

    // step 4: Validate / compute lightweight checksum
    num_groups = cre2_num_capturing_groups(re);
    err_code = cre2_error_code(re);
    computed_check = (long)easy_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length
                     + (long)num_groups + (long)err_code + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}