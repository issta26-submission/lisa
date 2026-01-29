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
//<ID> 98
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
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "extracted:\\1";
    const int rewrite_template_len = (int)(sizeof(rewrite_template) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t easy_match_result;
    cre2_string_t extract_target;
    int perl_flag = 0;
    int easy_res = 0;
    int extract_res = 0;
    int nmatch = 3;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&easy_match_result, 0, sizeof(easy_match_result));
    memset(&extract_target, 0, sizeof(extract_target));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = rewrite_template;
    rewrite_str.length = rewrite_template_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &easy_match_result, nmatch);
    extract_res = cre2_extract(pattern, &text_str, &rewrite_str, &extract_target);

    // step 5: Validate
    (void)perl_flag;
    (void)easy_res;
    (void)extract_res;
    (void)easy_match_result;
    (void)extract_target;
    (void)text_str;
    (void)rewrite_str;
    (void)pattern_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}