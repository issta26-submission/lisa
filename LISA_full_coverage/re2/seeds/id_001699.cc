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
//<ID> 1699
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char source_text[] = "foobarbaz";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[64];
    char target_buf[64];
    char rewrite_buf[] = "$1-$2";
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t pmatch[3];
    int rc_partial = 0;
    int rc_extract = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    text_str.data = text_buf;
    text_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    target_str.data = target_buf;
    target_str.length = 0;
    memset(pmatch, 0, sizeof(pmatch));
    rc_partial = cre2_partial_match(pattern, &text_str, pmatch, 3);
    rc_extract = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    err_code = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_partial + (long)rc_extract + (long)err_code + (long)enc + (long)text_str.length + (long)target_str.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}