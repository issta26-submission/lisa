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
//<ID> 10
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
    const char text[] = "this is test and another test";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_buf[] = "$1-checked";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char errmsg_buf[128];
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int posix_flag;
    int easy_res;
    int check_res;

    // step 2: Setup (configure options and prepare strings)
    memset(&match, 0, sizeof(match));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    cre2_opt_set_perl_classes(opt, 1);
    posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Operate (compile the pattern with options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (perform an easy match and check a rewrite string)
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Cleanup (destroy resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)posix_flag;
    (void)easy_res;
    (void)check_res;
    (void)match;
    (void)errmsg;

    return 66;
    // API sequence test completed successfully
}