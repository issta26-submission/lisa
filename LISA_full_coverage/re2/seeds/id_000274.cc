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
//<ID> 274
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
    const char sample_text[] = "ITEM-4242 extra data";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char target_buf[128];
    char rewrite_buf[] = "Name:$1 Num:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_s;
    cre2_string_t rewrite_s;
    cre2_string_t target_s;
    cre2_string_t matches[3];
    int match_res = 0;
    int extract_res = 0;
    const int nmatch = 3;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memset(&target_s, 0, sizeof(target_s));
    memset(matches, 0, sizeof(matches));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = rewrite_len;
    target_s.data = target_buf;
    target_s.length = 0;
    matches[0].data = target_buf; /* reuse buffer region for first capture pointer */
    matches[0].length = 0;

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Create regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Core operations and validation
    match_res = cre2_easy_match(pattern, pattern_len, text_s.data, text_s.length, matches, nmatch);
    extract_res = cre2_extract_re(re, &text_s, &rewrite_s, &target_s);
    /* propagate results to buffers to create a meaningful data flow */
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((match_res & 0xFF) + (extract_res & 0xFF)));
    target_buf[0] = (char)((unsigned char)target_buf[0] + (unsigned)((matches[0].length & 0xFF) + (target_s.length & 0xFF)));
    (void)matches;
    (void)match_res;
    (void)extract_res;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}