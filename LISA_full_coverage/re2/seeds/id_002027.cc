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
//<ID> 2027
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello ([A-Za-z]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "well hello world!";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    int match_ret = 0;
    int64_t max_mem_val = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t) (1 << 20)); // 1MB
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    memset(matches, 0, sizeof(matches));

    // step 3: Operate
    max_mem_val = cre2_opt_max_mem(opt);
    enc = cre2_opt_encoding(opt);
    match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Validate (simple checksum combining results)
    checksum = (long)pattern_len + (long)text_len + (long)match_ret + (long)max_mem_val + (long)enc + (long)(matches[0].length) + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}