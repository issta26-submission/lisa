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
//<ID> 2026
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)[0-9]+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text[] = "foo123baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    int easy_ret = 0;
    int64_t max_mem = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup - initialize match and create regexp with options
    match.data = NULL;
    match.length = 0;
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate - perform an easy match and query options
    easy_ret = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);
    max_mem = cre2_opt_max_mem(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Validate - compute a simple checksum to exercise data flow
    checksum = (long)pattern_len + (long)text_len + (long)easy_ret + (long)match.length + (long)max_mem + (long)enc + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}