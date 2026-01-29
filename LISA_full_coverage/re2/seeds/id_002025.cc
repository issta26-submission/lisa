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
//<ID> 2025
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "item42";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[64];
    cre2_string_t match;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int easy_ret = 0;
    int64_t observed_max_mem = 0;
    cre2_encoding_t observed_enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    memset(match_buf, 0, sizeof(match_buf));
    match.data = match_buf;
    match.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Configure
    observed_max_mem = cre2_opt_max_mem(opt);
    observed_enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)match.length + (long)easy_ret + (long)observed_max_mem + (long)observed_enc + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}