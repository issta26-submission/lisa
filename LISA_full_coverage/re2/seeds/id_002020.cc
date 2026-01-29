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
//<ID> 2020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "abcxyz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = NULL;
    cre2_string_t matches[2];
    int easy_ret = 0;
    int64_t maxmem = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup - configure options
    cre2_opt_set_max_mem(opt, (int64_t)(1024 * 1024));
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Operate - query options, perform easy match, compile regexp
    maxmem = cre2_opt_max_mem(opt);
    enc = cre2_opt_encoding(opt);
    matches[0].data = text_buf;
    matches[0].length = text_len;
    matches[1].data = NULL;
    matches[1].length = 0;
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 2);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Validate - compute a simple checksum using results
    checksum = (long)pattern_len + (long)text_len + (long)easy_ret + (long)maxmem + (long)enc + (long)(re != NULL) + (long)matches[0].length + (long)(matches[0].data != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}