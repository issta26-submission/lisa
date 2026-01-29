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
//<ID> 656
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
    cre2_string_t matches[3];
    int nmatch = 3;
    int rc_add = 0;
    int rc_match = 0;
    int one_line_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Configure
    one_line_flag = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add = cre2_set_add_simple(set, pattern);

    // step 4: Operate
    rc_match = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, nmatch);

    // step 5: Validate
    computed_check = rc_add + rc_match + one_line_flag + (int)enc + matches[0].length + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}