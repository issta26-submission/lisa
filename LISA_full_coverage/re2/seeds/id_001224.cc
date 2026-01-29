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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char partial_pattern[] = "hello([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    char text_buf[] = "hello123 world";
    const int text_len = (int)(sizeof(text_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;

    cre2_string_t input;
    cre2_string_t match;
    cre2_string_t partial_matches[4];

    int compile_ret = 0;
    int easy_ret = 0;
    int partial_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(&input, 0, sizeof(input));
    memset(&match, 0, sizeof(match));
    memset(partial_matches, 0, sizeof(partial_matches));
    input.data = text_buf;
    input.length = text_len;

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_add_simple(set, partial_pattern);

    // step 4: Core operations
    compile_ret = cre2_set_compile(set);
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, &match, 4);
    partial_ret = cre2_partial_match(partial_pattern, &input, partial_matches, 4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)partial_pattern_len + (long)text_len
                     + (long)compile_ret + (long)easy_ret + (long)partial_ret
                     + (long)match.length + (long)partial_matches[0].length
                     + (long)(re != NULL) + (long)(set != NULL) + (long)(iter != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}