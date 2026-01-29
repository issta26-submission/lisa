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
//<ID> 1692
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>ab)(cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "abcdXYZ";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char match_buf[64];
    char extract_target_buf[64];
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t text_str;
    cre2_string_t match_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    int rc_partial = 0;
    int rc_extract = 0;
    int errcode = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    memset(match_buf, 0, sizeof(match_buf));
    memset(extract_target_buf, 0, sizeof(extract_target_buf));
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    text_str.data = source_text;
    text_str.length = source_text_len;
    match_str.data = match_buf;
    match_str.length = 0;
    rc_partial = cre2_partial_match(pattern, &text_str, &match_str, 3);

    rewrite_str.data = "$1";
    rewrite_str.length = 2;
    target_str.data = extract_target_buf;
    target_str.length = 0;
    rc_extract = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    errcode = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_partial + (long)rc_extract
               + (long)enc + (long)errcode + (long)match_str.length + (long)target_str.length
               + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}