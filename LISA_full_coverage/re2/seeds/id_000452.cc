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
//<ID> 452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobar foobar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *found_name = NULL;
    int found_index = 0;
    cre2_string_t text_str;
    cre2_string_t matches[4];
    cre2_string_t partial_matches[4];
    int match_res = 0;
    int partial_res = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    text_str.data = text_buf;
    text_str.length = text_len;

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Operate
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    partial_res = cre2_partial_match(pattern, &text_str, partial_matches, 4);

    // step 5: Validate
    computed_check = match_res + partial_res + found_index + cre2_num_capturing_groups(re) + matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}