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
//<ID> 1199
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
    const char consume_pattern[] = "^(foo)([0-9]+)";
    const int consume_pattern_len = (int)(sizeof(consume_pattern) - 1);
    const char partial_pattern[] = "foo([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    char text_buf[] = "foo123 and foo45 foo999 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t input_str;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t partial_matches[4];
    cre2_string_t consume_matches[4];
    int partial_ret = 0;
    int consume_ret = 0;
    int replace_ret = 0;
    int greplace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&input_str, 0, sizeof(input_str));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(consume_matches, 0, sizeof(consume_matches));
    input_str.data = text_buf;
    input_str.length = text_len;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    re = cre2_new(pattern, pattern_len, NULL);

    // step 3: Core operations - partial match and consume
    partial_ret = cre2_partial_match(partial_pattern, &input_str, partial_matches, 2);
    consume_ret = cre2_consume(consume_pattern, &input_str, consume_matches, 2);

    // step 4: Core operations - replacements using compiled regexp
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);
    greplace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_ret + (long)consume_ret + (long)replace_ret + (long)greplace_ret
                     + (long)input_str.length + (long)text_and_target.length + (long)rewrite_str.length
                     + (long)partial_matches[0].length + (long)consume_matches[0].length + (long)(re != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}