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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t easy_match_out[3];
    cre2_string_t consume_input;
    cre2_string_t consume_out[3];
    cre2_string_t consume_re_input;
    cre2_string_t consume_re_out[3];
    int easy_res = 0;
    int consume_res = 0;
    int consume_re_res = 0;
    int log_errors_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    log_errors_flag = cre2_opt_log_errors(opt);
    memset(easy_match_out, 0, sizeof(easy_match_out));
    memset(consume_out, 0, sizeof(consume_out));
    memset(consume_re_out, 0, sizeof(consume_re_out));
    consume_input.data = text_buf;
    consume_input.length = text_len;
    consume_re_input.data = text_buf;
    consume_re_input.length = text_len;

    // step 3: Core operations
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_match_out, 3);
    re = cre2_new(pattern, pattern_len, opt);
    consume_res = cre2_consume(pattern, &consume_input, consume_out, 3);
    consume_re_res = cre2_consume_re(re, &consume_re_input, consume_re_out, 3);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)consume_res + (long)consume_re_res + (long)log_errors_flag + (long)easy_match_out[0].length + (long)consume_out[0].length + (long)consume_re_out[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}