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
//<ID> 1447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobarbazqux";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t matches[6];
    const int nmatch = 6;
    int ret_consume = 0;
    int lit_flag = 0;
    const char * pat_from_re = NULL;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    lit_flag = cre2_opt_literal(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    input.data = text;
    input.length = text_len;
    ret_consume = cre2_consume(pat_from_re, &input, matches, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)ret_consume + (long)lit_flag
               + (long)(opt != NULL) + (long)(re != NULL) + (long)matches[0].length + (long)matches[1].length
               + (long)(pat_from_re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}