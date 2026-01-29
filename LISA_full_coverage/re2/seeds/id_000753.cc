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
//<ID> 753
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
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t * re = NULL;
    struct cre2_string_t input;
    struct cre2_string_t matches[4];
    int rc_consume = 0;
    int computed_check = 0;
    const char * pat_from_re = NULL;

    // step 2: Configure
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Operate
    input.data = text_buf;
    input.length = text_len;
    memset(matches, 0, sizeof(matches));
    rc_consume = cre2_consume_re(re, &input, matches, 4);

    // step 4: Validate
    pat_from_re = cre2_pattern(re);
    computed_check = rc_consume + matches[0].length + matches[1].length + pattern_len;
    (void)pat_from_re;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}