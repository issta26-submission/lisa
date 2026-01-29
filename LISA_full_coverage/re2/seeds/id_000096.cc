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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(Hello) (World)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "Hello World!";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$2-$1";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t easy_match_result;
    int perl_flag = 0;
    int easy_res = 0;
    int extract_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(&easy_match_result, 0, sizeof(easy_match_result));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = rewrite_template;
    rewrite_str.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &easy_match_result, 3);
    extract_res = cre2_extract(pattern, &text_str, &rewrite_str, &target_str);

    // step 5: Validate
    (void)perl_flag;
    (void)easy_res;
    (void)extract_res;
    (void)easy_match_result;
    (void)target_str;
    (void)rewrite_str;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}