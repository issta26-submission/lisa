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
//<ID> 916
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char initial_text[] = "foo and foobar foo";
    const int initial_text_len = (int)(sizeof(initial_text) - 1);
    const char rewrite_literal[] = "X";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    char buf_text[64];
    char buf_rewrite[16];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t match_arr[1];
    cre2_string_t easy_match_arr[1];
    int match_res1 = 0;
    int match_res2 = 0;
    int replace_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    memset(buf_text, 0, sizeof(buf_text));
    memset(buf_rewrite, 0, sizeof(buf_rewrite));
    memcpy(buf_text, initial_text, (size_t)initial_text_len);
    memcpy(buf_rewrite, rewrite_literal, (size_t)(sizeof(rewrite_literal) - 1));
    text_and_target.data = buf_text;
    text_and_target.length = initial_text_len;
    rewrite.data = buf_rewrite;
    rewrite.length = (int)(sizeof(rewrite_literal) - 1);
    match_arr[0].data = NULL;
    match_arr[0].length = 0;
    easy_match_arr[0].data = NULL;
    easy_match_arr[0].length = 0;
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    match_res1 = cre2_match(re, buf_text, initial_text_len, 0, initial_text_len, CRE2_UNANCHORED, match_arr, 1);
    match_res2 = cre2_easy_match(pattern, pattern_len, buf_text, initial_text_len, easy_match_arr, 1);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate
    computed_check = (long)pattern_len + (long)match_res1 + (long)match_res2 + (long)replace_res + (long)text_and_target.length + (long)rewrite.length + (long)match_arr[0].length + (long)easy_match_arr[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}