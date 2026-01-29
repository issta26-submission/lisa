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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?(?P<name>baz)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarbazyyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *found_name = NULL;
    int found_index = 0;
    cre2_string_t matches[4];
    cre2_string_t pmatches[4];
    cre2_string_t text_wrapped;
    int rc_match = 0;
    int rc_partial = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(pmatches, 0, sizeof(pmatches));
    memset(&text_wrapped, 0, sizeof(text_wrapped));
    text_wrapped.data = text_buf;
    text_wrapped.length = text_len;

    // step 3: Initialize / Configure
    re = cre2_new(pattern, pattern_len, NULL);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, (int)(sizeof(matches) / sizeof(matches[0])));
    rc_partial = cre2_partial_match(pattern, &text_wrapped, pmatches, (int)(sizeof(pmatches) / sizeof(pmatches[0])));

    // step 5: Validate
    computed_check = rc_match + rc_partial + pattern_len + text_wrapped.length + (found_name ? found_index : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}