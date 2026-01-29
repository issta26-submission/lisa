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
//<ID> 210
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
    const char rewrite_literal[] = "$2-$1";
    const int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    char text_buf[128];
    char spare_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    int res_replace = 0;
    int res_easy = 0;
    int never_capture_flag = 0;
    cre2_encoding_t enc_before = CRE2_UNKNOWN;
    cre2_encoding_t enc_after = CRE2_UNKNOWN;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(spare_buf, 0, sizeof(spare_buf));
    memset(match_arr, 0, sizeof(match_arr));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memcpy(text_buf, original_text, (size_t)original_text_len);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc_before = cre2_opt_encoding(opt);
    cre2_opt_set_encoding(opt, enc_before);
    never_capture_flag = cre2_opt_never_capture(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    text_and_target.data = text_buf;
    text_and_target.length = original_text_len;
    rewrite_s.data = rewrite_literal;
    rewrite_s.length = rewrite_len;
    res_replace = cre2_replace_re(re, &text_and_target, &rewrite_s);
    res_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_and_target.length, &match_arr[0], 3);

    // step 4: Inspect results
    match_arr[0].length = match_arr[0].length + ((res_replace + res_easy + never_capture_flag) & 7);
    rewrite_s.length = rewrite_s.length + (match_arr[0].length & 3);
    text_and_target.length = text_and_target.length + (rewrite_s.length & 15);
    enc_after = cre2_opt_encoding(opt);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target.length & 0x7));
    spare_buf[0] = (char)((unsigned char)spare_buf[0] + ((unsigned)rewrite_s.length & 0x7));
    match_arr[1].length = match_arr[1].length + ((enc_before == enc_after) ? 1 : 0); /* no branching allowed, expression is constant-evaluable */

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)pattern_len;
    (void)res_replace;
    (void)res_easy;
    (void)never_capture_flag;
    (void)match_arr;
    (void)spare_buf;
    return 66;
}