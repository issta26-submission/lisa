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
//<ID> 67
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(?:-world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix hello-world suffix";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_text[] = "\\1_REWRITTEN";
    const int rewrite_len = (int)(sizeof(rewrite_text) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;

    cre2_string_t text_const;
    cre2_string_t text_mut;
    cre2_string_t matches[4];
    cre2_string_t rewrite;
    cre2_string_t errmsg;

    int partial_res = 0;
    int consume_res = 0;
    int check_res = 0;

    // step 2: Setup
    memset(&text_const, 0, sizeof(text_const));
    memset(&text_mut, 0, sizeof(text_mut));
    memset(matches, 0, sizeof(matches));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));

    text_const.data = text;
    text_const.length = text_len;

    text_mut.data = text;
    text_mut.length = text_len;

    rewrite.data = rewrite_text;
    rewrite.length = rewrite_len;

    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    partial_res = cre2_partial_match(pattern, &text_const, &matches[0], 4);
    consume_res = cre2_consume_re(re, &text_mut, &matches[0], 4);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    (void)partial_res;
    (void)consume_res;
    (void)check_res;
    (void)matches;
    (void)text_const;
    (void)text_mut;
    (void)rewrite;
    (void)errmsg;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}