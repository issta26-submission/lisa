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
//<ID> 456
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *found_name = NULL;
    int found_index = -1;
    bool iter_next_flag = false;
    cre2_string_t text_str;
    cre2_string_t matches[4];
    cre2_string_t partial_matches[4];
    int match_ret = 0;
    int partial_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    text_str.data = text_buf;
    text_str.length = text_len;
    memset(matches, 0, sizeof(matches));
    memset(partial_matches, 0, sizeof(partial_matches));

    // step 3: Initialize regex and iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate - named groups iteration, full match, partial match
    iter_next_flag = cre2_named_groups_iter_next(iter, &found_name, &found_index);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, 4);
    partial_ret = cre2_partial_match(pattern, &text_str, partial_matches, 4);

    // step 5: Validate / combine results into a checksum
    computed_check = match_ret + partial_ret + found_index + (int)iter_next_flag + cre2_num_capturing_groups(re);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}