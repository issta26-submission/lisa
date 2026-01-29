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
//<ID> 1448
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
    const char text[] = "xyz foobar baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t match;
    const char * pat_from_re = NULL;
    int consume_result = 0;
    int literal_flag = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    input.data = text;
    input.length = text_len;
    match.data = NULL;
    match.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    if (opt) {
        cre2_opt_set_literal(opt, 1);      // use setter (correct API)
        cre2_opt_set_encoding(opt, CRE2_UTF8);
    }

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        pat_from_re = cre2_pattern(re);
    }
    consume_result = cre2_consume(pat_from_re, &input, &match, 10);

    // step 5: Validate
    if (opt) {
        literal_flag = cre2_opt_literal(opt);
    } else {
        literal_flag = 0;
    }
    checksum = (long)pattern_len + (long)input.length + (long)match.length + (long)consume_result
               + (long)literal_flag + (long)(re != NULL) + (long)(pat_from_re != NULL);
    (void)checksum;

    // step 6: Cleanup
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}