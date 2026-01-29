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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(bc)?d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxabcdyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t input_sp;
    cre2_string_t match_sp;
    char set_err[128];
    int consume_res = 0;
    int add_res = 0;
    int err_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&match_sp, 0, sizeof(match_sp));
    memset(set_err, 0, sizeof(set_err));
    input_sp.data = text_buf;
    input_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);

    // step 4: Create regex and add to set
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    add_res = cre2_set_add(set, pattern, (size_t)pattern_len, set_err, sizeof(set_err));

    // step 5: Operate and Validate
    consume_res = cre2_consume(pattern, &input_sp, &match_sp, 1);
    computed_check = consume_res + add_res + err_code + match_sp.length + input_sp.length + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}