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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(\\w+)=(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "key=123 tail";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t input_sp;
    cre2_string_t matches[3];
    char error_buf[128];
    int set_add_res = 0;
    int consume_res = 0;
    int err_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(error_buf, 0, sizeof(error_buf));
    input_sp.data = text_buf;
    input_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    set_add_res = cre2_set_add(set, pattern, (size_t)pattern_len, error_buf, sizeof(error_buf));

    // step 4: Operate
    consume_res = cre2_consume(pattern, &input_sp, matches, 3);

    // step 5: Validate
    computed_check = set_add_res + consume_res + err_code + matches[0].length + matches[1].length + matches[2].length + pattern_len + text_len + (int)error_buf[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}