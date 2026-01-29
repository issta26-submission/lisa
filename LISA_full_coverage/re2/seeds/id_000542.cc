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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char subject_buf[] = "xxfoobarYY";
    const int subject_len = (int)(sizeof(subject_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_string_t matches[3];
    cre2_string_t subject;
    int nmatch = 3;
    int rc = 0;
    int one_line_flag = 0;
    int posix_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(&subject, 0, sizeof(subject));
    subject.data = subject_buf;
    subject.length = subject_len;
    opt = cre2_opt_new();

    // step 3: Configure
    one_line_flag = cre2_opt_one_line(opt);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 4: Operate
    rc = cre2_easy_match(pattern, pattern_len, subject.data, subject.length, matches, nmatch);

    // step 5: Validate
    computed_check = rc + one_line_flag + posix_flag + subject.length + matches[0].length + matches[1].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}