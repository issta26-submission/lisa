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
//<ID> 479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g1>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|bar";
    const char text_buf[] = "xxfoobar foo bar foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_str;
    cre2_range_t ranges[4];
    int nmatch = 4;
    int add_ret = 0;
    int compile_ret = 0;
    int match_ret = 0;
    int named_idx_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    memset(ranges, 0, sizeof(ranges));
    text_str.data = text_buf;
    text_str.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    add_ret = cre2_set_add_simple(set, set_pattern);
    compile_ret = cre2_set_compile(set);

    // step 4: Operate
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, nmatch);
    named_idx_ret = cre2_find_named_capturing_groups(re, "g1");
    cre2_strings_to_ranges(text_buf, ranges, match_arr, nmatch);

    // step 5: Validate
    computed_check = match_ret + named_idx_ret + add_ret + compile_ret + cre2_num_capturing_groups(re) + (int)ranges[0].start + (int)ranges[0].past + match_arr[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}