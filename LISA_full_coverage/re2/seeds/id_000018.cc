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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "(test)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "this is test and more test";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_s[] = "$1X";
    const int rewrite_len = (int)(sizeof(rewrite_s) - 1);
    char errmsg_buf[128];
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match[2];
    cre2_string_t rewrite;
    cre2_string_t errmsg;

    // step 2: Setup (configure options)
    cre2_opt_set_perl_classes(opt, 1);
    int posix_flag = cre2_opt_posix_syntax(opt);
    (void)posix_flag;

    // step 3: Operate (compile and easy match)
    re = cre2_new(pattern, pattern_len, opt);
    memset(match, 0, sizeof(match));
    int easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, match, 2);

    // step 4: Operate (prepare rewrite and check it against compiled regexp)
    rewrite.data = rewrite_s;
    rewrite.length = rewrite_len;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    int check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate and Cleanup (use results and free resources)
    (void)easy_res;
    (void)match;
    (void)check_res;
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
    // API sequence test completed successfully
}