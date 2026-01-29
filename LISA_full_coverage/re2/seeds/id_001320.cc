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
//<ID> 1320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabcdyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t text_string;
    int easy_ret = 0;
    int partial_ret = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(&text_string, 0, sizeof(text_string));
    text_string.data = text_buf;
    text_string.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, 3);
    partial_ret = cre2_partial_match(pattern, &text_string, match_arr, 3);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)easy_ret + (long)partial_ret
                     + (long)num_groups + (long)(re != NULL) + (long)(opt != NULL)
                     + (long)match_arr[0].length + (long)text_string.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}