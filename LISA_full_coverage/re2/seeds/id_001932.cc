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
//<ID> 1932
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int num_groups = 0;
    const char text[] = "foobar foobar";
    cre2_string_t text_and_target = { text, (int)(sizeof(text) - 1) };
    const char rewrite_pattern[] = "X";
    cre2_string_t rewrite = { rewrite_pattern, (int)(sizeof(rewrite_pattern) - 1) };
    int replace_res = -1;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_log_errors(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)num_groups + (long)replace_res + (long)(opt != NULL) + (long)(re != NULL) + (long)(text_and_target.data != NULL) + (long)(rewrite.data != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}