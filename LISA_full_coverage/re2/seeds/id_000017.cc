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
//<ID> 17
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
    const char text[] = "this is a test string";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_literal[] = "$1X";
    const int rewrite_len = (int)(sizeof(rewrite_literal) - 1);
    char errbuf[128];
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int posix_flag = 0;
    int easy_res = 0;
    int check_res = 0;

    // step 2: Setup (configure options and prepare buffers)
    memset(&match, 0, sizeof(match));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(errbuf, 0, sizeof(errbuf));
    if (opt) {
        cre2_opt_set_perl_classes(opt, 1);
        posix_flag = cre2_opt_posix_syntax(opt);
    }

    // step 3: Operate (compile regexp and perform easy match)
    re = cre2_new(pattern, pattern_len, opt);
    rewrite.data = rewrite_literal;
    rewrite.length = rewrite_len;
    errmsg.data = errbuf;
    errmsg.length = (int)sizeof(errbuf);
    easy_res = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);

    // step 4: Validate (check rewrite string against compiled regexp)
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Cleanup (release resources)
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize (use results to avoid unused warnings)
    (void)posix_flag;
    (void)easy_res;
    (void)check_res;
    (void)match;
    (void)errmsg;
    // API sequence test completed successfully
    return 66;
}