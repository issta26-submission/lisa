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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(item)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix item-4242 suffix";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[256];
    char target_buf[256];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_s;
    cre2_string_t rewrite_s;
    cre2_string_t target_s;
    const char * compiled_pat = NULL;
    int replace_res = 0;
    size_t set_matches = 0;
    int match_indices[8];
    int version_age = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    rewrite_s.data = "\\1-REPLACED";
    rewrite_s.length = (int)(sizeof("\\1-REPLACED") - 1);
    target_s.data = target_buf;
    target_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Configure / Create regex and set
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Core operations
    replace_res = cre2_replace(pattern, &text_s, &rewrite_s);
    set_matches = cre2_set_match(set, text_s.data, (size_t)text_s.length, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));
    version_age = cre2_version_interface_age();

    // step 5: Validate / propagate results
    {
        int cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
        target_s.length = (int)((replace_res & 0xFF) + (int)set_matches + (cp0 & 0x7) + (version_age & 0x3));
        if (target_s.length < 0) target_s.length = 0;
        target_s.length = (target_s.length > (int)(sizeof(target_buf) - 1)) ? (int)(sizeof(target_buf) - 1) : target_s.length;
        /* propagate a small effect into buffers to ensure data flow */
        text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((target_s.length) & 0x7));
        target_buf[0] = (char)((unsigned char)target_buf[0] + (unsigned)((replace_res) & 0x7));
        (void)match_indices;
        (void)cp0;
    }

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}