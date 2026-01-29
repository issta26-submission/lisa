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
//<ID> 1098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-z]+)([0-9]*)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "sample123 partialmatch test456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[8];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int pm_ret = 0;
    int set_compile_ret = 0;
    int group_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_longest_match(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations (create regexp and set, then compile set)
    re = cre2_new(pattern, pattern_len, opt);
    group_count = cre2_num_capturing_groups(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate (prepare match buffers and perform a partial match)
    memset(matches, 0, sizeof(matches));
    cre2_string_t text_str;
    text_str.data = text_buf;
    text_str.length = text_len;
    pm_ret = cre2_partial_match(pattern, &text_str, matches, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)group_count + (long)set_compile_ret + (long)pm_ret + (long)matches[0].length + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}