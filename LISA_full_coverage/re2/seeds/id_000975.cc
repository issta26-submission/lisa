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
//<ID> 975
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
    char text1[] = "alice123 bob456";
    const int text1_len = (int)(sizeof(text1) - 1);
    char text2[] = "foo99bar88";
    const int text2_len = (int)(sizeof(text2) - 1);
    char rewrite1_buf[128];
    char rewrite2_buf[128];
    cre2_string_t text_and_target1;
    cre2_string_t text_and_target2;
    cre2_string_t rewrite1;
    cre2_string_t rewrite2;
    cre2_string_t matches[3];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int easy_res = 0;
    int replace1_res = 0;
    int replace2_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    memset(matches, 0, sizeof(matches));
    easy_res = cre2_easy_match(pattern, pattern_len, text1, text1_len, matches, 3);
    memset(rewrite1_buf, 0, sizeof(rewrite1_buf));
    memset(rewrite2_buf, 0, sizeof(rewrite2_buf));
    text_and_target1.data = text1;
    text_and_target1.length = text1_len;
    rewrite1.data = rewrite1_buf;
    rewrite1.length = 0;
    replace1_res = cre2_global_replace(pattern, &text_and_target1, &rewrite1);
    text_and_target2.data = text2;
    text_and_target2.length = text2_len;
    rewrite2.data = rewrite2_buf;
    rewrite2.length = 0;
    replace2_res = cre2_global_replace_re(re, &text_and_target2, &rewrite2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)replace1_res + (long)replace2_res + (long)matches[0].length + (long)rewrite1.length + (long)rewrite2.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}