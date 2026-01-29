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
//<ID> 1220
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
    const char partial_pattern[] = "foo([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    char text_buf[] = "foo123 and bar45 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    cre2_string_t input;
    cre2_string_t partial_matches[4];
    cre2_string_t easy_match_slot;
    int easy_ret = 0;
    int partial_ret = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    memset(&input, 0, sizeof(input));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(&easy_match_slot, 0, sizeof(easy_match_slot));
    input.data = text_buf;
    input.length = text_len;
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, partial_pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 3: Core operations
    easy_ret = cre2_easy_match(partial_pattern, partial_pattern_len, text_buf, text_len, &easy_match_slot, 2);
    partial_ret = cre2_partial_match(partial_pattern, &input, partial_matches, 3);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)partial_pattern_len + (long)text_len
                     + (long)easy_ret + (long)partial_ret + (long)set_add_ret + (long)set_compile_ret
                     + (long)easy_match_slot.length + (long)partial_matches[0].length
                     + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}