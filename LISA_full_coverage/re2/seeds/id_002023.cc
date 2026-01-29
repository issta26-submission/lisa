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
//<ID> 2023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfoo123baryy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[2];
    int easy_ret = 0;
    int64_t opt_max = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    memset(&matches[0], 0, sizeof(matches));

    // step 3: Configure
    opt_max = cre2_opt_max_mem(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 2);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)easy_ret + (long)matches[0].length + (long)opt_max + (long)enc + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
    // API sequence test completed successfully
}