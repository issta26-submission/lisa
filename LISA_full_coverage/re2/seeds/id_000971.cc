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
//<ID> 971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text1[] = "alice123 bob456 alice789";
    const int text1_len = (int)(sizeof(text1) - 1);
    char text2[] = "foo alice123 bar alice456";
    const int text2_len = (int)(sizeof(text2) - 1);
    char rewrite_buf1[128];
    char rewrite_buf2[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target1;
    cre2_string_t rewrite1;
    cre2_string_t text_and_target2;
    cre2_string_t rewrite2;
    cre2_string_t matches[3];
    int easy_res = 0;
    int rep_res1 = 0;
    int rep_res2 = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Compile regexp and obtain named-groups iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Prepare match structures and perform an easy match
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    easy_res = cre2_easy_match(pattern, pattern_len, text1, text1_len, matches, 3);

    // step 5: Perform global replacements using both APIs (pattern and compiled regexp)
    memset(rewrite_buf1, 0, sizeof(rewrite_buf1));
    text_and_target1.data = text1;
    text_and_target1.length = text1_len;
    rewrite1.data = rewrite_buf1;
    rewrite1.length = 0;
    rep_res1 = cre2_global_replace(pattern, &text_and_target1, &rewrite1);

    memset(rewrite_buf2, 0, sizeof(rewrite_buf2));
    text_and_target2.data = text2;
    text_and_target2.length = text2_len;
    rewrite2.data = rewrite_buf2;
    rewrite2.length = 0;
    rep_res2 = cre2_global_replace_re(re, &text_and_target2, &rewrite2);

    // step 6: Validate / Cleanup
    computed_check = (long)pattern_len + (long)easy_res + (long)rep_res1 + (long)rep_res2 + (long)matches[0].length + (long)rewrite1.length + (long)rewrite2.length;
    (void)computed_check;
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}