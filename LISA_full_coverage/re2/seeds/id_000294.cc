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
//<ID> 294
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "item(\\d+)-X";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix item42-X suffix";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t match_s;
    const char * compiled_pat = NULL;
    int log_err_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int match_res = 0;
    int cp0 = 0;
    const int nmatch = 2;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    log_err_flag = cre2_opt_log_errors(opt);
    cre2_opt_set_literal(opt, (log_err_flag & 1));
    re = cre2_new(pattern, pattern_len, opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    match_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, &match_s, nmatch);
    match_s.length = match_s.length + ((int)enc & 0x3) + (match_res & 0x3);

    // step 5: Validate
    compiled_pat = re ? cre2_pattern(re) : NULL;
    cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((cp0 & 0xF) + (match_s.length & 0x7)));

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}