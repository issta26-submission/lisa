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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?(?P<name>baz)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoo barbar foobaz foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *found_name = NULL;
    int found_index = 0;
    cre2_string_t match_arr[4];
    cre2_string_t partial_inputs[1];
    cre2_string_t partial_outputs[3];
    int match_ret = 0;
    int partial_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(partial_inputs, 0, sizeof(partial_inputs));
    memset(partial_outputs, 0, sizeof(partial_outputs));
    partial_inputs[0].data = text_buf;
    partial_inputs[0].length = text_len;
    re = cre2_new(pattern, pattern_len, NULL);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Operate
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 4);
    partial_ret = cre2_partial_match(pattern, (const cre2_string_t *)partial_inputs, partial_outputs, 3);

    // step 4: Validate
    computed_check = match_ret + partial_ret + found_index + match_arr[0].length + partial_outputs[0].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // step 6: API sequence test completed successfully
    return 66;
}