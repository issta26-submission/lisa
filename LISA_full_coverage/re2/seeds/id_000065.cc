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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(?:-(world))?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix hello-world suffix";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t match1;
    cre2_string_t match2;
    cre2_string_t rewrite_str;
    cre2_string_t errmsg_str;
    char rewrite_buf[] = "$1:repl";
    char errmsg_buf[128];
    int res_partial = 0;
    int res_consume = 0;
    int res_check = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&match1, 0, sizeof(match1));
    memset(&match2, 0, sizeof(match2));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&errmsg_str, 0, sizeof(errmsg_str));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    errmsg_str.data = errmsg_buf;
    errmsg_str.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    res_partial = cre2_partial_match(pattern, &text_str, &match1, 3);
    res_consume = cre2_consume_re(re, &text_str, &match2, 3);
    res_check = cre2_check_rewrite_string(re, &rewrite_str, &errmsg_str);

    // step 5: Validate
    (void)res_partial;
    (void)res_consume;
    (void)res_check;
    (void)match1;
    (void)match2;
    (void)text_str;
    (void)rewrite_str;
    (void)errmsg_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}