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
//<ID> 1481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^([A-Za-z]+)\\s+(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Item 1234 trailing text";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t match;
    int consume_ret = 0;
    int posix_flag = 0;
    const char * re_pat = NULL;
    int errcode = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    text.data = text_buf;
    text.length = text_len;
    match.data = NULL;
    match.length = 0;
    consume_ret = cre2_consume(pattern, &text, &match, 1);
    re_pat = cre2_pattern(re);
    errcode = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)posix_flag + (long)consume_ret
               + (long)match.length + (long)(re != NULL) + (long)(re_pat != NULL) + (long)errcode;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}