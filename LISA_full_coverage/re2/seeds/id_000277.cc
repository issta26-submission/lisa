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
//<ID> 277
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
    const char sample_text[] = "ITEM-4242 trailing-info";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[64];
    char match_buf[64];
    char rewrite_buf[8];
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    cre2_string_t rewrite_s;
    cre2_string_t target_s;
    int match_res = 0;
    int extract_res = 0;
    const int nmatch = 3;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&match_s, 0, sizeof(match_s));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memset(&target_s, 0, sizeof(target_s));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    /* rewrite uses backreference 1 to extract the word part */
    memcpy(rewrite_buf, "\\1", (size_t)((int)(sizeof("\\1") - 1)));
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = (int)(sizeof("\\1") - 1);
    target_s.data = target_buf;
    target_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 0);

    // step 4: Create regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Core operations and validation
    match_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, &match_s, nmatch);
    /* propagate results: use match result to influence target buffer length for extract */
    target_s.length = (int)((match_res & 0xFF) + (match_s.length & 0xFF));
    /* call extract using compiled regex to fill target with rewrite of first capture */
    extract_res = cre2_extract_re(re, &text_s, &rewrite_s, &target_s);
    /* further propagate to buffers to ensure meaningful data flow */
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)extract_res & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)target_s.length & 0x7));
    (void)match_res;
    (void)extract_res;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    /* API sequence test completed successfully */
    return 66;
}