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
//<ID> 63
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
    const char rewrite[] = "\\1_REPL";
    const int rewrite_len = (int)(sizeof(rewrite) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t text_copy;
    cre2_string_t match;
    cre2_string_t match2;
    cre2_string_t rewrite_str;
    cre2_string_t errmsg;
    int partial_res = 0;
    int consume_res = 0;
    int check_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&text_copy, 0, sizeof(text_copy));
    memset(&match, 0, sizeof(match));
    memset(&match2, 0, sizeof(match2));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&errmsg, 0, sizeof(errmsg));
    text_str.data = text;
    text_str.length = text_len;
    text_copy.data = text;
    text_copy.length = text_len;
    rewrite_str.data = rewrite;
    rewrite_str.length = rewrite_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_res = cre2_partial_match(pattern, &text_str, &match, 2);
    consume_res = cre2_consume_re(re, &text_copy, &match2, 2);
    check_res = cre2_check_rewrite_string(re, &rewrite_str, &errmsg);

    // step 5: Validate
    (void)partial_res;
    (void)consume_res;
    (void)check_res;
    (void)match;
    (void)match2;
    (void)errmsg;
    (void)text_str;
    (void)text_copy;
    (void)rewrite_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}