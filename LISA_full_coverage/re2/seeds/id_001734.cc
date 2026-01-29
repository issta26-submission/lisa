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
//<ID> 1734
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "foobarbaz foobar foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[64];
    char rewrite_buf[32];
    const char rewrite_literal[] = "$1-$2";
    const int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    int replace_res = 0;
    int opt_perl_classes_get = 0;
    int opt_never_nl_get = 0;
    cre2_encoding_t opt_encoding_get = CRE2_UNKNOWN;
    int re_error = 0;
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
    memcpy(rewrite_buf, rewrite_literal, (size_t)rewrite_len);
    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 4: Operate
    replace_res = cre2_replace_re(re, &text_and_target_str, &rewrite_str);
    opt_perl_classes_get = cre2_opt_perl_classes(opt);
    opt_never_nl_get = cre2_opt_never_nl(opt);
    opt_encoding_get = cre2_opt_encoding(opt);
    re_error = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)replace_res + (long)opt_perl_classes_get + (long)opt_never_nl_get + (long)opt_encoding_get + (long)re_error + (long)text_and_target_str.length + (long)rewrite_str.length + (long)(text_and_target_buf[0]);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}