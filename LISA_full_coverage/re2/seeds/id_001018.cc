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
//<ID> 1018
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)\\s+([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice 123 bob 456 carol 789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[256];
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match_arr[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int match_res = 0;
    int replace_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    cre2_opt_set_word_boundary(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Compile pattern
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Perform an easy match against the text
    match_arr[0].data = NULL; match_arr[0].length = 0;
    match_arr[1].data = NULL; match_arr[1].length = 0;
    match_arr[2].data = NULL; match_arr[2].length = 0;
    match_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, 3);

    // step 5: Prepare rewrite buffer and perform global replace using compiled regexp
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = 0;
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate / Cleanup
    computed_check = (long)match_res + (long)replace_res + (long)text_and_target.length + (long)rewrite.length + (long)enc + (long)pattern_len;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}