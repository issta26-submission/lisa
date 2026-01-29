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
//<ID> 91
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
    const char input_text[] = "prefix foobar suffix";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    const char rewrite_template[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t match_result;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    char target_buf[128];
    int perl_flag = 0;
    int easy_res = 0;
    int extract_res = 0;
    int n_captures = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&match_result, 0, sizeof(match_result));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    text_str.data = input_text;
    text_str.length = input_text_len;
    rewrite_str.data = rewrite_template;
    rewrite_str.length = (int)(sizeof(rewrite_template) - 1);
    target_str.data = target_buf;
    target_str.length = (int)sizeof(target_buf);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    n_captures = cre2_num_capturing_groups(re);
    match_result.data = NULL;
    match_result.length = 0;
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &match_result, n_captures + 1);
    extract_res = cre2_extract(pattern, &text_str, &rewrite_str, &target_str);

    // step 5: Validate
    (void)perl_flag;
    (void)n_captures;
    (void)easy_res;
    (void)extract_res;
    (void)match_result;
    (void)target_str;
    (void)rewrite_str;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}