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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)(ing)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "this is testing of test cases";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_text[] = "$1_repl";
    const int rewrite_text_len = (int)(sizeof(rewrite_text) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int posix_flag = 0;
    int easy_res = 0;
    int check_res = 0;

    // step 2: Setup (configure options and clear structures)
    cre2_opt_set_perl_classes(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);
    memset(&match, 0, sizeof(match));
    memset(&errmsg, 0, sizeof(errmsg));
    rewrite.data = rewrite_text;
    rewrite.length = rewrite_text_len;

    // step 3: Operate (compile regexp with configured options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (easy match using pattern)
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);

    // step 5: Validate (check rewrite string against compiled regexp)
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)posix_flag;
    (void)easy_res;
    (void)check_res;
    (void)match;
    (void)errmsg;
    // API sequence test completed successfully
    return 66;
}