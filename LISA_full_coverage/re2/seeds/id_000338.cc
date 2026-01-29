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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)ef";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxabcdefyy";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t input;
    cre2_string_t match1;
    cre2_string_t match2;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int verrev = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int mcount1 = 0;
    int mcount2 = 0;
    int validate_sum = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(&match1, 0, sizeof(match1));
    memset(&match2, 0, sizeof(match2));
    opt = cre2_opt_new();
    verrev = cre2_version_interface_revision();
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    input.data = text;
    input.length = text_len;
    mcount1 = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, &match1, 3);
    mcount2 = cre2_find_and_consume_re(re, &input, &match2, 3);

    // step 5: Validate
    validate_sum = verrev + (int)enc + mcount1 + mcount2 + match1.length + match2.length;
    (void)validate_sum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}