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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(\\w+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "id-123 some other id-456";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$1:$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite;
    cre2_string_t extract_target;
    cre2_string_t match_arr[3];
    int perl_flag = 0;
    int easy_res = 0;
    int extract_res = 0;
    int nmatch = 3;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&extract_target, 0, sizeof(extract_target));
    memset(match_arr, 0, sizeof(match_arr));
    text_str.data = text;
    text_str.length = text_len;
    rewrite.data = rewrite_template;
    rewrite.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    perl_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, match_arr, nmatch);
    extract_res = cre2_extract(pattern, &text_str, &rewrite, &extract_target);

    // step 5: Validate
    (void)perl_flag;
    (void)easy_res;
    (void)extract_res;
    (void)match_arr;
    (void)extract_target;
    (void)text_str;
    (void)rewrite;
    (void)pattern_len;
    (void)text_len;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}