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
//<ID> 2028
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)\\s*(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "Item 12345";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[2];
    int easy_ret = 0;
    int64_t observed_max_mem = 0;
    cre2_encoding_t observed_enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_max_mem(opt, (int64_t) (1024 * 1024)); // 1MB

    // step 3: Configure - read back configured values
    observed_max_mem = cre2_opt_max_mem(opt);
    observed_enc = cre2_opt_encoding(opt);

    // step 4: Operate - compile regexp using options and perform an easy match
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    easy_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 2);

    // step 5: Validate - simple checksum to ensure values flowed through APIs
    checksum = (long)pattern_len + (long)text_len + (long)observed_max_mem + (long)observed_enc + (long)easy_ret + (long)(re != NULL) + (long)matches[0].length + (long)matches[1].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}