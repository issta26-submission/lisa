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
//<ID> 543
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject_buf[] = "xxfooYY";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_string_t matches[2];
    int nmatch = 2;
    int rc_easy = 0;
    int opt_one_line = 0;
    int opt_posix = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    opt_one_line = cre2_opt_one_line(opt);
    opt_posix = cre2_opt_posix_syntax(opt);

    // step 4: Operate
    rc_easy = cre2_easy_match(pattern, pattern_len, subject_buf, subject_len, matches, nmatch);

    // step 5: Validate
    computed_check = rc_easy + opt_one_line + opt_posix + matches[0].length + matches[1].length + subject_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}