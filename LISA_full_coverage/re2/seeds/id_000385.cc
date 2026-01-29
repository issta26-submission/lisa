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
//<ID> 385
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "\\bfox\\b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "the quick brown fox jumps";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_sp;
    cre2_string_t matches[2];
    char set_error[64];
    int set_add_res = 0;
    int consume_res = 0;
    int err_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(set_error, 0, sizeof(set_error));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);

    // step 4: Compile
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Operate
    /* cre2_set_new expects a cre2_anchor_t second argument; cast 0 to that type */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add(set, pattern, (size_t)pattern_len, set_error, sizeof(set_error));
    consume_res = cre2_consume(pattern, &text_sp, matches, 2);
    err_code = cre2_error_code(re);

    // step 6: Validate and Cleanup
    computed_check = set_add_res + consume_res + err_code + matches[0].length + matches[1].length + pattern_len + text_len + (int)set_error[0];
    (void)computed_check;
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}