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
//<ID> 12
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
    const char text[] = "this is test";
    const int text_len = (int)(sizeof(text) - 1);
    const char replacement[] = "$1X";
    const int replacement_len = (int)(sizeof(replacement) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t match;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    memset(&match, 0, sizeof(match));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));

    // step 2: Configure (options)
    cre2_opt_set_perl_classes(opt, 1);
    int posix_flag = cre2_opt_posix_syntax(opt);

    // step 3: Operate (compile and easy match)
    re = cre2_new(pattern, pattern_len, opt);
    int easy_result = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);

    // step 4: Prepare rewrite and validate it against the compiled regexp
    rewrite.data = replacement;
    rewrite.length = replacement_len;
    int check_result = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Cleanup (delete compiled regexp and options)
    cre2_delete(re);
    cre2_opt_delete(opt);

    (void)posix_flag;
    (void)easy_result;
    (void)check_result;
    (void)match;
    (void)errmsg;

    // API sequence test completed successfully
    return 66;
}