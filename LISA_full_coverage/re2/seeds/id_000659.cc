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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t match;
    int match_arr[4];
    size_t match_len = 4;
    size_t match_count = 0;
    int rc_easy = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int one_line_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    memset(match_arr, 0, sizeof(match_arr));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    one_line_flag = cre2_opt_one_line(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);

    // step 4: Operate
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);

    // step 5: Validate
    computed_check = rc_easy + (int)enc + one_line_flag + (int)match_count + match.length + match_arr[0] + match_arr[1];
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}