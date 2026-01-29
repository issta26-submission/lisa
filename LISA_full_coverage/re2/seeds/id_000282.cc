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
//<ID> 282
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
    const char sample_text[] = "foo123 and foo456 tail";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char rewrite_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_s;
    cre2_string_t rewrite_s;
    const char * compiled_pat = NULL;
    int replace_res = 0;
    size_t set_matches = 0;
    int match_arr[8];
    int version_age = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(&text_s, 0, sizeof(text_s));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    rewrite_s.data = "\\1-REPL";
    rewrite_s.length = (int)(sizeof("\\1-REPL") - 1);
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Create and compile regex and set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate with APIs
    set_matches = cre2_set_match(set, text_s.data, (size_t)text_s.length, match_arr, (size_t)(sizeof(match_arr)/sizeof(match_arr[0])));
    compiled_pat = cre2_pattern(re);
    version_age = cre2_version_interface_age();
    replace_res = cre2_replace(pattern, &text_s, &rewrite_s);

    // step 5: Validate / propagate results
    {
        int cp0 = compiled_pat ? (int)compiled_pat[0] : 0;
        text_s.length = text_s.length + (int)((set_matches & 0x7) + ((replace_res & 0x3) + (version_age & 0x3)));
        text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)cp0 & 0x7));
        rewrite_buf[0] = (char)((unsigned char)rewrite_buf[0] + ((unsigned)match_arr[0] & 0x7));
        (void)cp0;
        (void)set_matches;
        (void)replace_res;
        (void)version_age;
    }

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}