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
//<ID> 1730
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
    const char source_text[] = "foobarbaz foobar foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[128];
    char rewrite_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_res = 0;
    int err_code = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int never_nl_flag = 0;
    int perl_classes_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_never_nl(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1-$2", 5);
    text_and_target.data = text_and_target_buf;
    text_and_target.length = source_text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = 5;

    // step 4: Core operations
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);
    err_code = cre2_error_code(re);
    enc = cre2_opt_encoding(opt);
    never_nl_flag = cre2_opt_never_nl(opt);
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)replace_res + (long)err_code + (long)enc + (long)never_nl_flag + (long)perl_classes_flag + (long)text_and_target.length + (long)rewrite.length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}