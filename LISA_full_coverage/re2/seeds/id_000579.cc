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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t subject;
    cre2_string_t matches[3];
    int nmatch = 3;
    int rc_consume = 0;
    int rc_easy = 0;
    int errcode = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    subject.data = text_buf;
    subject.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, nmatch);
    rc_consume = cre2_consume_re(re, &subject, matches, nmatch);
    errcode = cre2_error_code(re);

    // step 5: Validate
    computed_check = rc_easy + rc_consume + errcode + matches[0].length + matches[1].length + subject.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}