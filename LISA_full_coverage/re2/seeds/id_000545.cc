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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "xx(a+)yy";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "prefix xxaaaa yy suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_string_t match;
    int rc_easy = 0;
    int one_line_flag = 0;
    int posix_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    one_line_flag = cre2_opt_one_line(opt);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 4: Operate
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 5: Validate
    computed_check = rc_easy + one_line_flag + posix_flag + match.length + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}