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
//<ID> 1515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern_re[] = "(\\d+)";
    const int pattern_re_len = (int)(sizeof(pattern_re) - 1);
    const char set_pattern[] = "abc";
    const char text_buf[] = "hello 123 world 456";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text;
    cre2_string_t match;
    cre2_string_t rewrite;
    int add_ret = 0;
    int enc = 0;
    int find_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&match, 0, sizeof(match));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    text.data = text_buf;
    text.length = (int)(sizeof(text_buf) - 1);
    rewrite.data = "NUM";
    rewrite.length = (int)(sizeof("NUM") - 1);

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = (int)cre2_opt_encoding(opt);
    add_ret = cre2_set_add_simple(set, set_pattern);

    // step 4: Core operations
    re = cre2_new(pattern_re, pattern_re_len, opt);
    find_ret = cre2_find_and_consume("\\d+", &text, &match, 1);
    replace_ret = cre2_replace_re(re, &text, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_re_len + (long)enc + (long)add_ret + (long)find_ret + (long)replace_ret
               + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL)
               + (long)text.length + (long)match.length + (long)rewrite.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}