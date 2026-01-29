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
//<ID> 1694
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "Widget-2048 extra";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char target_buf[128];
    char rewrite_buf[32];
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t matches[3];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int err_code = 0;
    int partial_rc = 0;
    int extract_rc = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);

    // step 4: Operate
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    memcpy(rewrite_buf, "$1:$2", 5);

    text_str.data = text_buf;
    text_str.length = sample_text_len;

    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 5;

    target_str.data = target_buf;
    target_str.length = 0;

    matches[0].data = target_buf;
    matches[0].length = 0;
    matches[1].data = target_buf + 32;
    matches[1].length = 0;
    matches[2].data = target_buf + 64;
    matches[2].length = 0;

    partial_rc = cre2_partial_match(pattern, &text_str, matches, 3);
    extract_rc = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)sample_text_len + (long)enc + (long)err_code + (long)partial_rc + (long)extract_rc + (long)target_str.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}