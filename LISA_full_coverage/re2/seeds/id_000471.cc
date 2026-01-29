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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|bar";
    const char text_buf[] = "xxfoobar foo bar foobar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t text_str;
    cre2_string_t match_arr[4];
    cre2_range_t ranges[4];
    const char *group_name = "g";
    int found_index = -1;
    int match_ret = 0;
    int compiled_flag = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_str.data = text_buf;
    text_str.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    compiled_flag = cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 4: Operate
    found_index = cre2_find_named_capturing_groups(re, group_name);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 4);
    cre2_strings_to_ranges(text_buf, ranges, match_arr, 4);

    // step 5: Validate
    computed_check = found_index + match_ret + (int)ranges[0].start + cre2_num_capturing_groups(re) + compiled_flag;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}