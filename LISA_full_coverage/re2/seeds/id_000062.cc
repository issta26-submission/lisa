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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "helloworld and hello";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_a;
    cre2_string_t text_b;
    cre2_string_t match_a;
    cre2_string_t match_b;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int partial_res = 0;
    int consume_res = 0;
    int check_res = 0;

    // step 2: Setup
    memset(&text_a, 0, sizeof(text_a));
    memset(&text_b, 0, sizeof(text_b));
    memset(&match_a, 0, sizeof(match_a));
    memset(&match_b, 0, sizeof(match_b));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    text_a.data = text;
    text_a.length = text_len;
    text_b.data = text;
    text_b.length = text_len;
    rewrite.data = "$1";
    rewrite.length = 2;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_res = cre2_partial_match(pattern, &text_a, &match_a, 3);
    consume_res = cre2_consume_re(re, &text_b, &match_b, 3);

    // step 5: Check rewrite and validation
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    (void)partial_res;
    (void)consume_res;
    (void)check_res;
    (void)match_a;
    (void)match_b;
    (void)text_a;
    (void)text_b;
    (void)rewrite;
    (void)errmsg;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}