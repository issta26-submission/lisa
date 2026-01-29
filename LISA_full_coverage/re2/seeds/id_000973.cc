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
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text1_buf[] = "alice123 bob456";
    const int text1_len = (int)(sizeof(text1_buf) - 1);
    char text2_buf[] = "foo1 bar2 foo3";
    const int text2_len = (int)(sizeof(text2_buf) - 1);
    char rewrite_buf1[128];
    char rewrite_buf2[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_and_target1;
    cre2_string_t rewrite1;
    cre2_string_t text_and_target2;
    cre2_string_t rewrite2;
    int easy_match_res = 0;
    int replace_res_uncompiled = 0;
    int replace_res_compiled = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Core operations
    match_arr[0].data = NULL; match_arr[0].length = 0;
    match_arr[1].data = NULL; match_arr[1].length = 0;
    match_arr[2].data = NULL; match_arr[2].length = 0;
    match_arr[3].data = NULL; match_arr[3].length = 0;
    easy_match_res = cre2_easy_match(pattern, pattern_len, text1_buf, text1_len, match_arr, 4);

    memset(rewrite_buf1, 0, sizeof(rewrite_buf1));
    text_and_target1.data = text2_buf;
    text_and_target1.length = text2_len;
    rewrite1.data = rewrite_buf1;
    rewrite1.length = 0;
    replace_res_uncompiled = cre2_global_replace("([A-Za-z]+)([0-9]+)", &text_and_target1, &rewrite1);

    memset(rewrite_buf2, 0, sizeof(rewrite_buf2));
    text_and_target2.data = text1_buf;
    text_and_target2.length = text1_len;
    rewrite2.data = rewrite_buf2;
    rewrite2.length = 0;
    replace_res_compiled = cre2_global_replace_re(re, &text_and_target2, &rewrite2);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_match_res + (long)replace_res_uncompiled + (long)replace_res_compiled + (long)match_arr[0].length + (long)pattern_len;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66; // API sequence test completed successfully
}