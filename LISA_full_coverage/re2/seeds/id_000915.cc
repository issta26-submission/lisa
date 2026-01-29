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
//<ID> 915
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
    const char text[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text) - 1);
    const char repl_text[] = "REPL";
    const int repl_text_len = (int)(sizeof(repl_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[4];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int res_easy = 0;
    int res_match = 0;
    int res_replace = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    res_easy = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 4);
    res_match = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    text_and_target.data = text;
    text_and_target.length = text_len;
    rewrite.data = repl_text;
    rewrite.length = repl_text_len;
    res_replace = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)res_easy + (long)res_match + (long)res_replace + (long)cre2_num_capturing_groups(re) + (long)matches[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}