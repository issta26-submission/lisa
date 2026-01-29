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
//<ID> 758
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "^(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_buf[] = "foo bar baz";
    const int input_len = (int)(sizeof(input_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    const int MAX_MATCH = 4;
    cre2_string_t matches[MAX_MATCH];
    int rc_consume = 0;
    int computed_check = 0;
    const char * pat_from_re = NULL;

    // step 2: Setup (Configure)
    input.data = input_buf;
    input.length = input_len;
    memset(matches, 0, sizeof(matches));

    // step 3: Operate (Create regexp and consume)
    re = cre2_new(pattern, pattern_len, NULL);
    rc_consume = cre2_consume_re(re, &input, matches, MAX_MATCH);
    pat_from_re = cre2_pattern(re);

    // step 4: Validate (derive a simple checksum using outputs)
    computed_check = rc_consume + input.length + matches[0].length + matches[1].length + (int)pat_from_re[0] + pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}