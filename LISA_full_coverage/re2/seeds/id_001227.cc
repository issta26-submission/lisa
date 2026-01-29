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
//<ID> 1227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(user)([0-9]+):([a-z]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char partial_pattern[] = "user([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    char text_buf[] = "user123:alice and user45:bob extra";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t input;
    cre2_string_t easy_match_results[4];
    cre2_string_t partial_matches[4];
    int easy_match_ret = 0;
    int partial_ret = 0;
    int set_compile_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&input, 0, sizeof(input));
    memset(easy_match_results, 0, sizeof(easy_match_results));
    memset(partial_matches, 0, sizeof(partial_matches));
    input.data = text_buf;
    input.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure / Compile set
    set_compile_ret = cre2_set_compile(set);

    // step 4: Core operations
    iter = cre2_named_groups_iter_new(re);
    easy_match_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_match_results, 4);
    partial_ret = cre2_partial_match(partial_pattern, &input, partial_matches, 4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)partial_pattern_len + (long)text_len
                     + (long)easy_match_ret + (long)partial_ret + (long)set_compile_ret
                     + (long)easy_match_results[0].length + (long)partial_matches[0].length
                     + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}