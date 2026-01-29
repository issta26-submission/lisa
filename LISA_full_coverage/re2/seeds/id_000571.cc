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
//<ID> 571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxabcYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    cre2_string_t easy_matches[3];
    cre2_string_t consume_matches[3];
    cre2_string_t subject;
    int rc_easy = 0;
    int rc_consume = 0;
    int errcode = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(consume_matches, 0, sizeof(consume_matches));
    subject.data = text_buf;
    subject.length = text_len;

    // step 3: Initialize / Operate
    rc_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 3);
    re = cre2_new(pattern, pattern_len, (const cre2_options_t *)NULL);
    rc_consume = cre2_consume_re(re, &subject, consume_matches, 3);
    errcode = cre2_error_code(re);

    // step 4: Validate
    computed_check = rc_easy + rc_consume + errcode + easy_matches[0].length + easy_matches[1].length + consume_matches[0].length + subject.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // step 6: API sequence test completed successfully
    return 66;
}