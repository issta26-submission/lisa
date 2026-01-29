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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hi)(.*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "hi there, general kenobi";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "re:$1";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t partial_match;
    cre2_string_t consume_match;
    cre2_string_t rewrite_str;
    cre2_string_t errmsg;
    int partial_res = 0;
    int consume_res = 0;
    int check_res = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&partial_match, 0, sizeof(partial_match));
    memset(&consume_match, 0, sizeof(consume_match));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&errmsg, 0, sizeof(errmsg));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = rewrite_template;
    rewrite_str.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_res = cre2_partial_match(pattern, &text_str, &partial_match, 2);
    consume_res = cre2_consume_re(re, &text_str, &consume_match, 2);
    check_res = cre2_check_rewrite_string(re, &rewrite_str, &errmsg);

    // step 5: Validate
    (void)partial_res;
    (void)consume_res;
    (void)check_res;
    (void)partial_match;
    (void)consume_match;
    (void)errmsg;
    (void)rewrite_str;
    (void)text_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}