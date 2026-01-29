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
//<ID> 1517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t match;
    const char text_buf[] = "zzz foo yyy foo";
    const char rewrite_buf[] = "X";
    int add_ret = 0;
    int replace_ret = 0;
    int find_ret = 0;
    cre2_encoding_t enc;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    add_ret = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);
    find_ret = cre2_find_and_consume(pattern, &text_and_target, &match, 1);

    // step 5: Validate
    checksum = (long)pattern_len + (long)enc + (long)add_ret + (long)replace_ret + (long)find_ret
               + (long)(set != NULL) + (long)(re != NULL) + (long)match.length + (long)text_and_target.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}