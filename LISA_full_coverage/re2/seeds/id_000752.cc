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
//<ID> 752
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
    char text_buf[] = "foo bar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    const char * pattern_from_re = NULL;
    int rc_consume = 0;
    int computed_check = 0;

    // step 2: Setup
    input.data = text_buf;
    input.length = text_len;
    match.data = NULL;
    match.length = 0;
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Operate
    pattern_from_re = cre2_pattern(re);
    rc_consume = cre2_consume_re(re, &input, &match, 0);

    // step 4: Validate
    computed_check = (int)pattern_from_re[0] + pattern_len + input.length + match.length + rc_consume;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}