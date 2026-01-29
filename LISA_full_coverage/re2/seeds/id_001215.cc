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
//<ID> 1215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char input_buf[] = "foo123 and foo45 tail";
    const int input_len = (int)(sizeof(input_buf) - 1);
    char target_buf[] = "foo123 and foo45 tail";
    const int target_len = (int)(sizeof(target_buf) - 1);
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
    set = NULL;
    re = NULL;
    memset(&input, 0, sizeof(input));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(consume_matches, 0, sizeof(consume_matches));
    input.data = input_buf;
    input.length = input_len;
    text_and_target.data = target_buf;
    text_and_target.length = target_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 3: Configure
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    consume_ret = cre2_consume_re(re, &input, consume_matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)consume_ret + (long)replace_ret + (long)input.length + (long)text_and_target.length
                     + (long)rewrite_str.length + (long)consume_matches[0].length + (long)(re != NULL)
                     + (long)(set != NULL) + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}