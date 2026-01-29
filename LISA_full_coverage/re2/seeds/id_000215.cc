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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_literal[] = "$2-$1";
    const int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    char text_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_s;
    cre2_string_t matches[4];
    int match_res = 0;
    int replace_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int never_cap = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_s, 0, sizeof(rewrite_s));
    memset(matches, 0, sizeof(matches));
    memcpy(text_buf, input_text, (size_t)input_text_len);
    text_and_target.data = text_buf;
    text_and_target.length = input_text_len;
    rewrite_s.data = rewrite_literal;
    rewrite_s.length = rewrite_len;
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    never_cap = cre2_opt_never_capture(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    match_res = cre2_easy_match(pattern, pattern_len, text_buf, input_text_len, &matches[0], 4);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite_s);

    // step 4: Inspect results
    matches[0].length = matches[0].length + ((match_res + replace_res) & 7);
    text_and_target.length = text_and_target.length + ((matches[0].length + ((int)enc)) & 3);
    rewrite_s.length = rewrite_s.length + (never_cap & 1);

    // step 5: Validate / propagate results
    text_buf[0] = (char)((unsigned char)text_buf[0] + ((unsigned)text_and_target.length & 0x7));
    text_buf[1] = (char)((unsigned char)text_buf[1] + ((unsigned)rewrite_s.length & 0x7));
    (void)matches;
    (void)match_res;
    (void)replace_res;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}