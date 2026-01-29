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
//<ID> 1514
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
    cre2_string_t match;
    cre2_string_t rewrite;
    char text_buf[] = "foo bar foo";
    char match_buf[64];
    char rewrite_buf[] = "X";
    int add_ret = 0;
    int find_ret = 0;
    int replace_ret = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&match, 0, sizeof(match));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Core operations
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_ret = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);
    match.data = match_buf;
    match.length = 0;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    find_ret = cre2_find_and_consume(pattern, &text_and_target, &match, 0);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)find_ret + (long)replace_ret + (long)enc
               + (long)(set != NULL) + (long)(re != NULL)
               + (long)text_and_target.length + (long)match.length + (long)rewrite.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}