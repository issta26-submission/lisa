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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "foo-123 extra foo-456";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[256];
    char target_buf[256];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    int replace_res = 0;
    size_t set_matches = 0;
    int match_indices[8];
    const char * compiled_pat = NULL;
    int ver_age = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = sample_text_len;
    rewrite_s.data = "\\1-REPL";
    rewrite_s.length = (int)(sizeof("\\1-REPL") - 1);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Create regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Create and compile a set containing the pattern
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 5: Core operations — perform replace and then set matching and inspect compiled pattern & version age
    replace_res = cre2_replace(pattern, &text_and_target, &rewrite_s);
    set_matches = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, match_indices, (size_t)8);
    compiled_pat = cre2_pattern(re);
    ver_age = cre2_version_interface_age();

    // step 6: Inspect / propagate results and cleanup
    {
        int cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
        text_and_target.length = text_and_target.length + ((replace_res & 0xF) + (int)set_matches);
        text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)cp0 & 0x7));
        target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)ver_age & 0x7));
        match_indices[0] = match_indices[0] + (replace_res & 0x3);
        (void)cp0;
        (void)replace_res;
        (void)set_matches;
        (void)match_indices;
        (void)compiled_pat;
        (void)ver_age;
    }
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}