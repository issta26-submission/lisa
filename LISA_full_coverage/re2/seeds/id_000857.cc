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
//<ID> 857
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx helloworld yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t consume_in;
    cre2_string_t consume_out[4];
    cre2_string_t easy_matches[3];
    int consume_res1 = 0;
    int consume_res2 = 0;
    int easy_match_res = 0;
    int log_errors_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(&consume_in, 0, sizeof(consume_in));
    memset(consume_out, 0, sizeof(consume_out));
    memset(easy_matches, 0, sizeof(easy_matches));
    consume_in.data = text_buf;
    consume_in.length = text_len;
    consume_res1 = cre2_consume(pattern, &consume_in, consume_out, 4);
    consume_res2 = cre2_consume_re(re, &consume_in, consume_out, 4);
    easy_match_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 3);

    // step 4: Validate
    computed_check = (long)log_errors_flag + (long)consume_res1 + (long)consume_res2 + (long)easy_match_res + (long)easy_matches[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}