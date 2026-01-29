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
//<ID> 2197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "Hello";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Hello world!";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t match;
    cre2_options_t * opt = NULL;
    int case_flag = 0;
    int longest_flag = 0;
    int never_nl_flag = 0;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);
    case_flag = cre2_opt_case_sensitive(opt);
    longest_flag = cre2_opt_longest_match(opt);
    never_nl_flag = cre2_opt_never_nl(opt);

    // step 4: Operate
    match.data = NULL;
    match.length = 0;
    match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)match_ret + (long)case_flag + (long)longest_flag + (long)never_nl_flag + (long)(opt != NULL) + (long)match.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}