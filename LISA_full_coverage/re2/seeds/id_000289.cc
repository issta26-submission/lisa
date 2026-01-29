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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "prefix foo123 suffix foo456";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char rewrite_buf[] = "BAR$1";
    int match_array[8];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    const char * compiled_pat = NULL;
    int replace_res = 0;
    size_t match_count = 0;
    int ver_age = 0;
    int cp0 = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(match_array, 0, sizeof(match_array));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = sample_text_len;
    rewrite_s.data = rewrite_buf;
    rewrite_s.length = (int)(sizeof(rewrite_buf) - 1);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Configure / Create
    re = cre2_new(pattern, pattern_len, opt);
    compiled_pat = cre2_pattern(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate / Validate
    replace_res = cre2_replace(pattern, &text_and_target, &rewrite_s);
    match_count = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, match_array, (size_t)(sizeof(match_array)/sizeof(match_array[0])));
    ver_age = cre2_version_interface_age();
    cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
    text_and_target.length = text_and_target.length + (int)((replace_res & 0x7) + (int)(match_count & 0x7) + (ver_age & 0x3));
    text_buf[0] = (char)((unsigned char)text_buf[0] + (unsigned)((cp0 & 0xF) + (int)(match_count & 0x3)));

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}