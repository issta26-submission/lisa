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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "item-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "item-42 and item-84 tail";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[256];
    char target_buf[256];
    char rewrite_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target_s;
    cre2_string_t rewrite_s;
    int match_array[8];
    size_t set_matches = 0;
    const char * compiled_pattern = NULL;
    int replace_res = 0;
    int vi_age = 0;
    int cp0 = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_and_target_s.data = text_buf;
    text_and_target_s.length = sample_text_len;
    rewrite_s.data = "\\1-REPLACED";
    rewrite_s.length = (int)(sizeof("\\1-REPLACED") - 1);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Create / Configure regex and set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Core operations
    replace_res = cre2_replace(pattern, &text_and_target_s, &rewrite_s);
    set_matches = cre2_set_match(set, text_and_target_s.data, (size_t)text_and_target_s.length, match_array, (size_t)(sizeof(match_array) / sizeof(match_array[0])));
    compiled_pattern = cre2_pattern(re);
    vi_age = cre2_version_interface_age();

    // step 5: Inspect / propagate results
    cp0 = compiled_pattern ? (int)compiled_pattern[0] : 0;
    text_and_target_s.length = text_and_target_s.length + (int)(set_matches & 0xFF) + ((replace_res & 0x7) + (vi_age & 0x3));
    rewrite_s.length = rewrite_s.length + ((int)cp0 & 0xF);
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target_s.length & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)rewrite_s.length & 0x7));
    (void)match_array;
    (void)cp0;
    (void)vi_age;
    (void)replace_res;
    (void)set_matches;
    (void)compiled_pattern;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}