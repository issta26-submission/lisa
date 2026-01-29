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
//<ID> 1691
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "helloworld hello world";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char rewrite_buf[8];
    char target_buf[128];
    char match_buf0[32];
    char match_buf1[32];
    char match_buf2[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t matches[3];
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int partial_rc = 0;
    int extract_rc = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(match_buf0, 0, sizeof(match_buf0));
    memset(match_buf1, 0, sizeof(match_buf1));
    memset(match_buf2, 0, sizeof(match_buf2));
    memcpy(rewrite_buf, "$1", 2);

    text_str.data = source_text;
    text_str.length = source_text_len;

    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 2;

    target_str.data = target_buf;
    target_str.length = (int)sizeof(target_buf);

    matches[0].data = match_buf0;
    matches[0].length = 0;
    matches[1].data = match_buf1;
    matches[1].length = 0;
    matches[2].data = match_buf2;
    matches[2].length = 0;

    partial_rc = cre2_partial_match(pattern, &text_str, matches, 3);
    extract_rc = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    err_code = cre2_error_code(re);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    checksum = (long)pattern_len + (long)partial_rc + (long)extract_rc + (long)err_code + (long)enc
               + (long)text_str.length + (long)target_str.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}