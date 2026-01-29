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
//<ID> 281
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
    const char sample_text[] = "item-4242 trailing";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char rewrite_buf[64];
    int match_indices[8];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    const char * compiled_pat = NULL;
    int replace_res = 0;
    size_t set_matches = 0;
    int ver_age = 0;
    int cp0 = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(match_indices, 0, sizeof(match_indices));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = sample_text_len;
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = 0;
    memcpy(rewrite_buf, "REPL", (size_t)4);
    rewrite_s.length = 4;

    // step 3: Initialize and configure options
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Create compiled regex and inspect pattern
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    cp0 = compiled_pat ? (int)compiled_pat[0] : 0;

    // step 5: Operate - perform a single replace and a set-based match
    replace_res = cre2_replace(pattern, &text_and_target, &rewrite_s);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_matches = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));
    ver_age = cre2_version_interface_age();

    // step 6: Validate / propagate results and cleanup
    text_and_target.length = (int)((int)text_and_target.length + ((replace_res & 0x7) + (ver_age & 0x3)));
    rewrite_s.length = (int)(rewrite_s.length + ((int)set_matches & 0x7));
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)rewrite_s.length & 0x7));
    rewrite_buf[0] = (char)((unsigned char)rewrite_buf[0] + ((unsigned)cp0 & 0x7));
    (void)match_indices;
    (void)replace_res;
    (void)set_matches;
    (void)ver_age;
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}