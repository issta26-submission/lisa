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
//<ID> 1480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)( world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello world and more";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text;
    cre2_string_t matches[2];
    const char * pattern_from_re = NULL;
    int posix_flag = 0;
    int consume_ret = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&text, 0, sizeof(text));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    text.data = text_buf;
    text.length = text_len;
    pattern_from_re = cre2_pattern(re);
    consume_ret = cre2_consume(pattern_from_re, &text, matches, 2);
    err_code = cre2_error_code(re);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)posix_flag + (long)consume_ret
               + (long)matches[0].length + (long)matches[1].length + (long)(pattern_from_re != NULL)
               + (long)err_code + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}