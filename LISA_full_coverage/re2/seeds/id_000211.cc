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
//<ID> 211
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
    const char original_text[] = "prefix foobar suffix";
    const int original_text_len = (int)(sizeof(original_text) - 1);
    char text_buf[128];
    char target_buf[128];
    const char rewrite_literal[] = "$2-$1";
    const int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_s;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    int match_res = 0;
    int replace_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int never_capture_flag = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&match_s, 0, sizeof(match_s));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    never_capture_flag = cre2_opt_never_capture(opt);
    re = cre2_new(pattern, pattern_len, opt);
    memcpy(text_buf, original_text, (size_t)original_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = original_text_len;
    rewrite_s.data = rewrite_literal;
    rewrite_s.length = rewrite_len;

    // step 3: Configure
    match_s.data = NULL;
    match_s.length = 0;

    // step 4: Core operations
    match_res = cre2_easy_match(pattern, pattern_len, text_and_target.data, text_and_target.length, &match_s, 1);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite_s);

    // step 5: Inspect / Validate
    match_s.length = match_s.length + ((match_res + replace_res) & 3);
    text_and_target.length = text_and_target.length + ((replace_res + match_res + never_capture_flag + (int)enc) & 7);
    target_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)(text_and_target.length & 0x7));
    (void)target_buf;
    (void)rewrite_s;
    (void)pattern_len;
    (void)original_text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}