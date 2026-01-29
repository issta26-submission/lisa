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
//<ID> 2193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t matches[3];
    cre2_options_t * opt = NULL;
    int longest_flag = 0;
    int never_nl_flag = 0;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);

    // step 3: Configure
    longest_flag = cre2_opt_longest_match(opt);
    never_nl_flag = cre2_opt_never_nl(opt);

    // step 4: Operate
    match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);

    // step 5: Validate
    checksum = (long)pattern_len + (long)match_ret + (long)longest_flag + (long)never_nl_flag + (long)matches[0].length + (long)matches[1].length + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}