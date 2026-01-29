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
//<ID> 910
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)+c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "abababc";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_string_t easy_matches[4];
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
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations - prepare match targets and perform easy match
    memset(easy_matches, 0, sizeof(easy_matches));
    easy_matches[0].data = NULL;
    easy_matches[0].length = 0;
    res_easy = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 4);

    // step 4: Core operations - perform match with compiled regexp and then a replace using the same compiled regexp
    memset(matches, 0, sizeof(matches));
    matches[0].data = NULL;
    matches[0].length = 0;
    res_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);

    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    res_replace = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)res_easy + (long)res_match + (long)res_replace + (long)cre2_num_capturing_groups(re) + (long)easy_matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}