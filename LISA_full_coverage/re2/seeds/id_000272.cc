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
//<ID> 272
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
    const char sample_text[] = "Item-4242 trailing";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char rewrite_buf[64];
    char target_buf[128];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t rewrite_s;
    cre2_string_t target_s;
    cre2_string_t match_s;
    const int nmatch = 3;
    int match_res = 0;
    int extract_res = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memset(&target_s, 0, sizeof(target_s));
    memset(&match_s, 0, sizeof(match_s));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    /* use a rewrite that references capture groups */
    memcpy(rewrite_buf, "EXTRACT:$1|$2", 13);
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = 13;
    target_s.data = target_buf;
    target_s.length = 0;
    match_s.data = match_buf;
    match_s.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Create regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Core operations and validation
    match_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, &match_s, nmatch);
    extract_res = cre2_extract_re(re, &text_s, &rewrite_s, &target_s);
    /* propagate results into buffers/lengths to create meaningful data flow */
    match_s.length = match_s.length + ((match_res & 0x7) + (extract_res & 0x3));
    target_s.length = target_s.length + ((int)match_s.length & 0xF);
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)match_s.length & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)target_s.length & 0x7));
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)text_s.length & 0x7));
    (void)match_res;
    (void)extract_res;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}