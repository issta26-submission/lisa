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
//<ID> 654
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello world";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t match;
    int rc_add_simple = 0;
    int rc_easy_match = 0;
    int one_line_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add_simple = cre2_set_add_simple(set, pattern);
    one_line_flag = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    rc_easy_match = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 5: Validate
    computed_check = rc_add_simple + rc_easy_match + one_line_flag + (int)enc + match.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}