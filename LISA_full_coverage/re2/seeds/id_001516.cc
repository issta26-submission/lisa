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
//<ID> 1516
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t consume_text;
    cre2_string_t consume_match;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int add_ret = 0;
    int replace_ret = 0;
    int consume_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&consume_text, 0, sizeof(consume_text));
    memset(&consume_match, 0, sizeof(consume_match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 4: Core operations
    add_ret = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);
    static char textbuf[] = "Value: 12345 end";
    static char rewrite_src[] = "NUM";
    text_and_target.data = textbuf;
    text_and_target.length = (int)(sizeof(textbuf) - 1);
    rewrite.data = rewrite_src;
    rewrite.length = (int)(sizeof(rewrite_src) - 1);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);
    static char consumebuf[] = "12345xyz";
    consume_text.data = consumebuf;
    consume_text.length = (int)(sizeof(consumebuf) - 1);
    consume_ret = cre2_find_and_consume("[0-9]+", &consume_text, &consume_match, 0);

    // step 5: Validate
    checksum = (long)pattern_len + (long)add_ret + (long)replace_ret + (long)consume_ret
               + (long)(enc) + (long)(set != NULL) + (long)(re != NULL)
               + (long)text_and_target.length + (long)consume_match.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}