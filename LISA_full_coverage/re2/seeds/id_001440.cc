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
//<ID> 1440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)|(baz)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobar and baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t matches[6];
    const int nmatch = 6;
    int ret_consume = 0;
    int literal_flag = 0;
    const char * pat_from_re = NULL;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    literal_flag = cre2_opt_literal(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    text_str.data = text;
    text_str.length = text_len;
    ret_consume = cre2_consume(pattern, &text_str, matches, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)ret_consume + (long)literal_flag
               + (long)matches[0].length + (long)(re != NULL) + (long)(opt != NULL)
               + (long)(pat_from_re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}