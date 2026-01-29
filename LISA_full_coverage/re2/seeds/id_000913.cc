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
//<ID> 913
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
    cre2_string_t easy_matches[4];
    cre2_string_t match_results[4];
    char subject[] = "foobar xyz foo";
    const int subject_len = (int)(sizeof(subject) - 1);
    char text_and_target_buf[] = "replace foo with X: foo";
    cre2_string_t text_and_target = { text_and_target_buf, (int)(sizeof(text_and_target_buf) - 1) };
    char rewrite_buf[] = "X";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    int easy_res = 0;
    int match_res = 0;
    int replace_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations (compile, easy match, match, replace)
    re = cre2_new(pattern, pattern_len, opt);
    memset(easy_matches, 0, sizeof(easy_matches));
    easy_res = cre2_easy_match(pattern, pattern_len, subject, subject_len, easy_matches, 3);
    memset(match_results, 0, sizeof(match_results));
    match_res = cre2_match(re, subject, subject_len, 0, subject_len, CRE2_UNANCHORED, match_results, 4);
    replace_res = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)easy_res + (long)match_res + (long)replace_res + (long)cre2_num_capturing_groups(re) + (long)text_and_target.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}