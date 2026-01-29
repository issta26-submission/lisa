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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char sample_text[] = "abc abc abc";
    const int sample_text_len = (int)(sizeof(sample_text) - 1);
    char text_buf[128];
    char rewrite_buf[64];
    char match_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_s;
    cre2_string_t rewrite_s;
    cre2_string_t dummy_s;
    const char * compiled_pat = NULL;
    int match_array[8];
    size_t set_match_count = 0;
    int replace_res = 0;
    int version_age = 0;
    const size_t match_array_len = sizeof(match_array) / sizeof(match_array[0]);

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memset(match_buf, 0, sizeof(match_buf));
    memcpy(text_buf, sample_text, (size_t)sample_text_len);
    text_s.data = text_buf;
    text_s.length = sample_text_len;
    rewrite_s.data = "\\1-X";
    rewrite_s.length = (int)(sizeof("\\1-X") - 1);
    dummy_s.data = match_buf;
    dummy_s.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Configure / Create
    re = cre2_new(pattern, pattern_len, opt);
    /* cre2_set_new expects a cre2_anchor_t for the second argument; pass a zero value casted to that type */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Core operations
    replace_res = cre2_replace(pattern, &text_s, &rewrite_s);
    set_match_count = cre2_set_match(set, text_s.data, (size_t)text_s.length, match_array, match_array_len);
    compiled_pat = cre2_pattern(re);
    version_age = cre2_version_interface_age();

    // step 5: Validate / propagate results
    dummy_s.length = dummy_s.length + ((replace_res & 0x7) + ((int)set_match_count & 0x3));
    text_s.length = text_s.length - ((int)dummy_s.length & 0xF);
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)version_age & 0x7));
    if (compiled_pat) { /* avoid unused variable warning without branching logic */ dummy_s.length = dummy_s.length + (int)compiled_pat[0]; }
    match_buf[0] = (char)((unsigned char)match_buf[0] + ((unsigned)text_s.length & 0x7));
    (void)replace_res;
    (void)set_match_count;
    (void)compiled_pat;
    (void)version_age;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}