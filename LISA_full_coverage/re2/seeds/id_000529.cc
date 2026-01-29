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
//<ID> 529
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
    char text_and_target_buf[] = "foo and foo again";
    char rewrite_buf[] = "bar";
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[3];
    cre2_string_t errarg;
    int nmatch = 3;
    int partial_rc = 0;
    int replace_rc = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&errarg, 0, sizeof(errarg));
    subject.data = subject_buf;
    subject.length = subject_len;
    text_and_target.data = text_and_target_buf;
    text_and_target.length = (int)(sizeof(text_and_target_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // step 3: Create regexp
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Operate with partial match and global replace, and retrieve error arg
    partial_rc = cre2_partial_match_re(re, &subject, matches, nmatch);
    cre2_error_arg(re, &errarg);
    replace_rc = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate results (combine values to ensure meaningful flow)
    computed_check = partial_rc + replace_rc + matches[0].length + matches[1].length + text_and_target.length + errarg.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}