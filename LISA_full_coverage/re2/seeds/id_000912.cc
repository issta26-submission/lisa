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
//<ID> 912
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    char text_buf[] = "foobar foo";
    int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text_and_target = { text_buf, text_len };
    cre2_string_t rewrite = { "X", 1 };
    cre2_string_t match_easy[2];
    cre2_string_t match_re[3];
    int easy_res = 0;
    int match_res = 0;
    int replace_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    memset(match_easy, 0, sizeof(match_easy));
    easy_res = cre2_easy_match("foo", 3, text_buf, text_len, match_easy, 2);
    memset(match_re, 0, sizeof(match_re));
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_re, 3);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)easy_res + (long)match_res + (long)replace_res + (long)match_re[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}