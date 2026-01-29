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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)\\b(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "prefix foo123 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_sp;
    cre2_string_t match_sp;
    char set_err[128];
    int consume_res = 0;
    int set_add_res = 0;
    int err_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(set_err, 0, sizeof(set_err));
    memset(&match_sp, 0, sizeof(match_sp));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    set_add_res = cre2_set_add(set, pattern, (size_t)pattern_len, set_err, sizeof(set_err));
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    consume_res = cre2_consume(pattern, &text_sp, &match_sp, 1);
    err_code = cre2_error_code(re);

    // step 5: Validate
    computed_check = consume_res + set_add_res + err_code + match_sp.length + (int)set_err[0] + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}