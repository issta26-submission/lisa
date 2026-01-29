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
//<ID> 19
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
    const char text[] = "this is testing and test only";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "${1}_REPL";
    const int rewrite_template_len = (int)(sizeof(rewrite_template) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    char errmsg_buf[128];

    // step 2: Setup (initialize structures and configure options)
    memset(&match, 0, sizeof(match));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    cre2_opt_set_perl_classes(opt, 1);
    int posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Operate (compile the pattern with configured options)
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate (perform an easy match using the same pattern)
    int easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);

    // step 5: Validate (prepare rewrite and check it against the compiled regexp)
    rewrite.data = rewrite_template;
    rewrite.length = rewrite_template_len;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    int check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 6: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    (void)posix_flag;
    (void)easy_res;
    (void)check_res;
    (void)match;
    (void)rewrite;
    (void)errmsg;
    return 66;
}