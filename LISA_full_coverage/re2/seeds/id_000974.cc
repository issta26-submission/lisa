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
//<ID> 974
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
    char text_buf1[] = "alice123 bob456 alice789";
    const int text_len1 = (int)(sizeof(text_buf1) - 1);
    char text_buf2[] = "carol12 dave34 eve56";
    const int text_len2 = (int)(sizeof(text_buf2) - 1);
    char rewrite_buf1[128];
    char rewrite_buf2[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target1;
    cre2_string_t text_and_target2;
    cre2_string_t rewrite1;
    cre2_string_t rewrite2;
    cre2_string_t matches[4];
    int easy_res = 0;
    int global_res1 = 0;
    int global_res2 = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    memset(rewrite_buf1, 0, sizeof(rewrite_buf1));
    memset(rewrite_buf2, 0, sizeof(rewrite_buf2));
    text_and_target1.data = text_buf1;
    text_and_target1.length = text_len1;
    text_and_target2.data = text_buf2;
    text_and_target2.length = text_len2;
    rewrite1.data = rewrite_buf1;
    rewrite1.length = 0;
    rewrite2.data = rewrite_buf2;
    rewrite2.length = 0;
    matches[0].data = NULL;
    matches[0].length = 0;
    matches[1].data = NULL;
    matches[1].length = 0;
    matches[2].data = NULL;
    matches[2].length = 0;
    matches[3].data = NULL;
    matches[3].length = 0;
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf1, text_len1, matches, 3);
    global_res1 = cre2_global_replace(pattern, &text_and_target1, &rewrite1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    global_res2 = cre2_global_replace_re(re, &text_and_target2, &rewrite2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)global_res1 + (long)global_res2 + (long)text_and_target1.length + (long)text_and_target2.length + (long)rewrite1.length + (long)rewrite2.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}