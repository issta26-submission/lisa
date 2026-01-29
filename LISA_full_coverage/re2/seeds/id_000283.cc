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
//<ID> 283
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
    const char sample_text[] = "item-4242 some extra data";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char target_buf[128];
    char rewrite_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_s;
    cre2_string_t target_s;
    cre2_string_t rewrite_s;
    const char * compiled_pat = NULL;
    int replace_res = 0;
    size_t set_match_count = 0;
    int set_matches[8];
    int api_age = 0;

    // step 2: Setup (Initialize and configure)
    memset(text_buf, 0, sizeof(text_buf));
    memset(target_buf, 0, sizeof(target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&target_s, 0, sizeof(target_s));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    target_s.data = target_buf;
    target_s.length = 0;
    memcpy(rewrite_buf, "\\1-REPLACED", (size_t)(sizeof("\\1-REPLACED") - 1));
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = (int)(sizeof("\\1-REPLACED") - 1);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Operate (Compile, replace, set-match)
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    replace_res = cre2_replace(pattern, &text_s, &rewrite_s);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    set_match_count = cre2_set_match(set, text_s.data, (size_t)text_s.length, set_matches, (size_t)(sizeof(set_matches)/sizeof(set_matches[0])));

    // step 4: Validate (propagate and inspect results)
    api_age = cre2_version_interface_age();
    text_s.length = text_s.length + (int)(replace_res & 0x7);
    target_s.length = target_s.length + (int)(set_match_count & 0x7);
    if (compiled_pat) { /* intentionally empty to create a use of compiled_pat without branching behavior */ }
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)api_age & 0x7));
    target_buf[0] = (char)((unsigned char)target_buf[0] + ((unsigned)text_s.length & 0x7));
    (void)compiled_pat;
    (void)set_matches;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}