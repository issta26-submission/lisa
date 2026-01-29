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
//<ID> 854
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
    cre2_string_t easy_matches[3];
    cre2_string_t consume_in;
    cre2_string_t consume_out_pattern[3];
    cre2_string_t consume_out_re[4];
    int easy_res = 0;
    int consume_res_pat = 0;
    int consume_res_re = 0;
    int log_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_flag = cre2_opt_log_errors(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations - easy match using pattern string
    memset(easy_matches, 0, sizeof(easy_matches));
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 3);

    // step 4: Core operations - consume using pattern string
    memset(&consume_in, 0, sizeof(consume_in));
    memset(consume_out_pattern, 0, sizeof(consume_out_pattern));
    consume_in.data = text_buf;
    consume_in.length = text_len;
    consume_res_pat = cre2_consume(pattern, &consume_in, consume_out_pattern, 3);

    // step 5: Core operations - consume using compiled regexp
    memset(consume_out_re, 0, sizeof(consume_out_re));
    memset(&consume_in, 0, sizeof(consume_in));
    consume_in.data = text_buf;
    consume_in.length = text_len;
    consume_res_re = cre2_consume_re(re, &consume_in, consume_out_re, 4);

    // step 6: Validate / Cleanup
    computed_check = (long)log_flag + (long)easy_res + (long)consume_res_pat + (long)consume_res_re + (long)easy_matches[0].length;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}