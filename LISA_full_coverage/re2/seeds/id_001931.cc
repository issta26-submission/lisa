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
//<ID> 1931
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
    const char original_text[] = "foobarbaz";
    const char rewrite_text[] = "X";
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_result = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    text_and_target.data = original_text;
    text_and_target.length = (int)(sizeof(original_text) - 1);
    rewrite.data = rewrite_text;
    rewrite.length = (int)(sizeof(rewrite_text) - 1);
    replace_result = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)num_groups + (long)replace_result + (long)text_and_target.length + (long)rewrite.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}