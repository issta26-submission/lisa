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
//<ID> 2155
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
    char text_buf[] = "Alice123 Bob42";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t matches[3];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    char text_and_target_buf[] = "Alice123 and Bob42";
    cre2_string_t text_and_target = { text_and_target_buf, (int)(sizeof(text_and_target_buf) - 1) };
    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int easy_ret = 0;
    int add_ret = 0;
    int compile_ret = 0;
    int replace_ret = 0;
    size_t set_match_count = 0;
    int match_arr[4];
    memset(match_arr, 0, sizeof(match_arr));
    long checksum = 0;

    // step 2: Setup (create and configure options and set)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Compile pattern into regexp and add to set
    re = cre2_new(pattern, pattern_len, opt);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Operate (easy match, replace using compiled regexp, and set match)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);
    set_match_count = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, match_arr, (size_t)(sizeof(match_arr) / sizeof(match_arr[0])));

    // step 5: Validate (combine values so results flow through)
    checksum = (long)pattern_len + (long)easy_ret + (long)replace_ret + (long)add_ret + (long)compile_ret + (long)set_match_count + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)text_and_target.length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}