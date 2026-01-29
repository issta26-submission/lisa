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
//<ID> 850
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
    cre2_string_t in;
    cre2_string_t out1[4];
    cre2_string_t out2[4];
    cre2_string_t match;
    int easy_res = 0;
    int consume_res = 0;
    int consume_re_res = 0;
    int log_errors_flag = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 3: Create regexp object
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Prepare input/output structures
    memset(&in, 0, sizeof(in));
    memset(out1, 0, sizeof(out1));
    memset(out2, 0, sizeof(out2));
    memset(&match, 0, sizeof(match));
    in.data = text_buf;
    in.length = text_len;

    // step 5: Core operations — easy match, consume by pattern, consume by regexp
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 2);
    consume_res = cre2_consume(pattern, &in, out1, 4);
    consume_re_res = cre2_consume_re(re, &in, out2, 4);

    // step 6: Validate and cleanup
    computed_check = (long)log_errors_flag + (long)easy_res + (long)consume_res + (long)consume_re_res + (long)match.length + (long)in.length;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}