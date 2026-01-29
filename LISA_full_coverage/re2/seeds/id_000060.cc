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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "abxab";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t match_str;
    cre2_string_t rewrite_str;
    cre2_string_t errmsg_str;
    int partial_res = 0;
    int consume_res = 0;
    int check_rewrite_res = 0;
    int nmatch = 3;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&match_str, 0, sizeof(match_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&errmsg_str, 0, sizeof(errmsg_str));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = "$1$2";
    rewrite_str.length = (int)(sizeof("$1$2") - 1);
    errmsg_str.data = NULL;
    errmsg_str.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_res = cre2_partial_match(pattern, &text_str, &match_str, nmatch);
    consume_res = cre2_consume_re(re, &text_str, &match_str, nmatch);
    check_rewrite_res = cre2_check_rewrite_string(re, &rewrite_str, &errmsg_str);

    // step 5: Validate
    (void)partial_res;
    (void)consume_res;
    (void)check_rewrite_res;
    (void)match_str;
    (void)errmsg_str;
    (void)rewrite_str;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}