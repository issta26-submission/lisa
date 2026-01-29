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
//<ID> 750
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
    char text_buf[] = "foobar and more text";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    const char * got_pattern = NULL;
    int rc_consume = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    input.data = text_buf;
    input.length = text_len;

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, NULL);
    got_pattern = cre2_pattern(re);

    // step 4: Operate
    rc_consume = cre2_consume_re(re, &input, &match, 2);

    // step 5: Validate
    computed_check = rc_consume + match.length + input.length + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}