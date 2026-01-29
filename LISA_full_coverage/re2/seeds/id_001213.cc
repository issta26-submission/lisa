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
//<ID> 1213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 and bar456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t consume_matches[4];
    int consume_ret = 0;
    int replace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(&input, 0, sizeof(input));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(consume_matches, 0, sizeof(consume_matches));
    input.data = text_buf;
    input.length = text_len;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    consume_ret = cre2_consume_re(re, &input, consume_matches, 4);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)consume_ret + (long)replace_ret
                     + (long)consume_matches[0].length + (long)text_and_target.length
                     + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}