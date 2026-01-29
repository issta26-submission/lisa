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
//<ID> 917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a|b)+c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text[] = "aaabbbc";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t matches[4];
    cre2_string_t rewrite;
    cre2_string_t text_and_target;
    int easy_res = 0;
    int match_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(matches, 0, sizeof(matches));
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 4);
    match_res = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    rewrite.data = "X";
    rewrite.length = 1;
    text_and_target.data = text;
    text_and_target.length = text_len;
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)easy_res + (long)match_res + (long)replace_res + (long)text_and_target.length;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}