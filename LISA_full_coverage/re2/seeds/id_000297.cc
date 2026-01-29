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
//<ID> 297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "LITERAL.*(X)"; 
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix LITERAL.*(X) suffix";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int orig_log_errors = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int match_res = 0;
    const int nmatch = 1;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure and create regex
    orig_log_errors = cre2_opt_log_errors(opt);
    cre2_opt_set_literal(opt, 1);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);

    // step 4: Operate and validate
    match_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, &match_s, nmatch);
    match_s.length = match_s.length + ((match_res & 0x7) + (orig_log_errors & 0x3) + ((int)enc & 0x3));
    text_s.length = text_s.length - ((int)match_s.length & 0xF);
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned char)((compiled_pat && compiled_pat[0]) ? (int)compiled_pat[0] & 0x7 : 0));
    (void)compiled_pat;
    (void)match_res;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}