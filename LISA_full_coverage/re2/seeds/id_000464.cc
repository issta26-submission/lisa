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
//<ID> 464
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
    const char set_pattern[] = "foo|bar|baz";
    const char text_buf[] = "foobarbaz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text_str;
    cre2_string_t match_arr[6];
    int easy_ret = 0;
    int consume_ret = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int error_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_str.data = text_buf;
    text_str.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    set_add_ret = cre2_set_add_simple(set, set_pattern);
    set_compile_ret = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, match_arr, 6);
    consume_ret = cre2_consume_re(re, &text_str, match_arr, 6);
    error_code = cre2_error_code(re);

    // step 5: Validate
    computed_check = easy_ret + consume_ret + set_add_ret + set_compile_ret + error_code + (int)text_str.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}