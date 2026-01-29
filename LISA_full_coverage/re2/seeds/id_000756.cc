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
//<ID> 756
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foobar baz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t matches[4];
    int rc_consume = 0;
    int computed_check = 0;
    const char * pat_from_re = NULL;

    // step 2: Setup (Configure)
    memset(matches, 0, sizeof(matches));
    input.data = text_buf;
    input.length = text_len;
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Operate
    rc_consume = cre2_consume_re(re, &input, matches, 3);

    // step 4: Validate
    pat_from_re = cre2_pattern(re);
    computed_check = rc_consume + matches[0].length + matches[1].length + pattern_len + (int)pat_from_re[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}