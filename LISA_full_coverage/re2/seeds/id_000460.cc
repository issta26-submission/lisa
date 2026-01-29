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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char easy_pat[] = "world";
    const int easy_pat_len = (int)(sizeof(easy_pat) - 1);
    const char text_buf[] = "hello world!";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text_str;
    cre2_string_t match_arr_easy[4];
    cre2_string_t match_arr_consume[4];
    int matches[8];
    int easy_ret = 0;
    int consume_ret = 0;
    int errcode = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t set_match_count = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr_easy, 0, sizeof(match_arr_easy));
    memset(match_arr_consume, 0, sizeof(match_arr_consume));
    memset(matches, 0, sizeof(matches));
    text_str.data = text_buf;
    text_str.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set_add_ret = cre2_set_add_simple(set, easy_pat);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate
    easy_ret = cre2_easy_match(easy_pat, easy_pat_len, text_buf, text_len, match_arr_easy, 4);
    consume_ret = cre2_consume_re(re, &text_str, match_arr_consume, 4);
    errcode = cre2_error_code(re);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, matches, (size_t)(sizeof(matches) / sizeof(matches[0])));

    // step 5: Validate
    computed_check = easy_ret + consume_ret + errcode + set_add_ret + set_compile_ret + (int)set_match_count + cre2_num_capturing_groups(re);
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}