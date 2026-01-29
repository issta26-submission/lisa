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
//<ID> 1327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\b(foo)(bar)?\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[2];
    cre2_string_t input_str;
    cre2_string_t partial_out;
    int easy_ret = 0;
    int partial_ret = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(&input_str, 0, sizeof(input_str));
    memset(&partial_out, 0, sizeof(partial_out));
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, 2);
    input_str.data = text_buf;
    input_str.length = text_len;
    partial_ret = cre2_partial_match(pattern, &input_str, &partial_out, 1);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)easy_ret + (long)partial_ret
                     + (long)num_groups + (long)(re != NULL) + (long)match_arr[0].length + (long)partial_out.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}