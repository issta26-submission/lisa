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
//<ID> 1486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)\\s+(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello   world and more";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    const char * compiled_pattern = NULL;
    int posix_flag = 0;
    int consume_ret = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    input.data = text_buf;
    input.length = text_len;
    match.data = NULL;
    match.length = 0;
    consume_ret = cre2_consume(pattern, &input, &match, 2);
    compiled_pattern = cre2_pattern(re);
    err_code = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)posix_flag + (long)consume_ret
               + (long)match.length + (long)err_code + (long)(re != NULL) + (long)(compiled_pattern != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}