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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize inputs and containers)
    const char pattern[] = "^(test)([0-9]*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "test123 and other text";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int easy_res = 0;
    int check_res = 0;
    int posix_flag = 0;

    // step 2: Setup (configure options)
    cre2_opt_set_perl_classes(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Operate (compile regexp with configured options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (use easy match to extract first capturing group)
    memset(&match, 0, sizeof(match));
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);

    // step 5: Validate (check a rewrite string against compiled regexp)
    rewrite.data = "$1";
    rewrite.length = 2;
    memset(&errmsg, 0, sizeof(errmsg));
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)posix_flag;
    (void)easy_res;
    (void)check_res;
    (void)match;
    (void)rewrite;
    (void)errmsg;
    // API sequence test completed successfully
    return 66;
}