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
//<ID> 1015
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)_(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice_123 bob_456 carol_789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[64];
    char rewrite_buf[256];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int easy_res = 0;
    int replace_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations - lightweight match using easy API
    match.data = match_buf;
    match.length = 0;
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 1);

    // step 4: Core operations - compile regexp and perform global replace with compiled regexp
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    rewrite.data = rewrite_buf;
    rewrite.length = 0;
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)replace_res + (long)enc + (long)match.length + (long)rewrite.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}