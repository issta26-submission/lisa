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
//<ID> 851
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
    const char simple_pattern[] = "foo";
    char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t consume_in;
    cre2_string_t consume_out[3];
    cre2_string_t easy_match_out[3];
    cre2_string_t consume_nonre_out[3];
    int res_consume_re = 0;
    int res_easy_match = 0;
    int res_consume = 0;
    int log_errors_flag = 0;
    int rev = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    log_errors_flag = cre2_opt_log_errors(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(&consume_in, 0, sizeof(consume_in));
    memset(consume_out, 0, sizeof(consume_out));
    memset(easy_match_out, 0, sizeof(easy_match_out));
    memset(consume_nonre_out, 0, sizeof(consume_nonre_out));
    consume_in.data = text_buf;
    consume_in.length = text_len;
    res_consume_re = cre2_consume_re(re, &consume_in, consume_out, 3);
    res_easy_match = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_match_out, 3);
    res_consume = cre2_consume(simple_pattern, &consume_in, consume_nonre_out, 3);

    // step 4: Validate
    rev = cre2_version_interface_revision();
    computed_check = (long)res_consume_re + (long)res_easy_match + (long)res_consume + (long)rev + (long)log_errors_flag + (long)consume_out[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}