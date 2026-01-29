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
//<ID> 1442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo)(bar)?(baz)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobarbaz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    const int nmatch = 6;
    cre2_string_t match[nmatch];
    const char * patt_from_re = NULL;
    int consume_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match, 0, sizeof(match));
    input.data = text;
    input.length = text_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    patt_from_re = cre2_pattern(re);
    consume_ret = cre2_consume(patt_from_re, &input, match, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)consume_ret
               + (long)match[0].length + (long)(patt_from_re != NULL)
               + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}