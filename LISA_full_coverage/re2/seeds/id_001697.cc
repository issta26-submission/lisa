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
//<ID> 1697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "helloworld and more";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char rewrite_buf[32];
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t matches[4];
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int rc_partial = 0;
    int rc_extract = 0;
    int errcode = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    text_str.data = source_text;
    text_str.length = source_text_len;
    memset(matches, 0, sizeof(matches));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(target_buf, 0, sizeof(target_buf));
    /* prepare a simple rewrite that references groups */
    rewrite_buf[0] = '$';
    rewrite_buf[1] = '1';
    rewrite_buf[2] = '-';
    rewrite_buf[3] = '$';
    rewrite_buf[4] = '2';
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 5;
    target_str.data = target_buf;
    target_str.length = 0;

    rc_partial = cre2_partial_match(pattern, &text_str, matches, 3);
    rc_extract = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    errcode = cre2_error_code(re);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_str.length + (long)rc_partial + (long)rc_extract + (long)errcode + (long)enc + (long)rewrite_str.length + (long)target_str.length + (long)matches[0].length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}