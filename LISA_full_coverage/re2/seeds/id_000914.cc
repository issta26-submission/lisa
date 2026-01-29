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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "xx foobar yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char mutable_text_buf[32];
    char rewrite_buf[] = "baz";
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t easy_match_res[4];
    cre2_string_t match_res[4];
    int r_easy = 0;
    int r_match = 0;
    int r_replace = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    memset(mutable_text_buf, 0, sizeof(mutable_text_buf));
    memcpy(mutable_text_buf, text_buf, (size_t)text_len);
    text_and_target.data = mutable_text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    memset(easy_match_res, 0, sizeof(easy_match_res));
    memset(match_res, 0, sizeof(match_res));
    re = cre2_new(pattern, pattern_len, opt);
    r_easy = cre2_easy_match("foo", 3, mutable_text_buf, text_len, easy_match_res, 2);
    r_match = cre2_match(re, mutable_text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_res, 3);
    r_replace = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)r_easy + (long)r_match + (long)r_replace + (long)cre2_num_capturing_groups(re) + (long)pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}