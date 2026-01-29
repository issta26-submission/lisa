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
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>a)(b)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int one_line_flag = 0;
    int num_groups = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    strings[0].data = text_buf + 2; strings[0].length = 1;
    strings[1].data = text_buf + 3; strings[1].length = 1;
    ranges[0].start = 0; ranges[0].past = 0;
    ranges[1].start = 0; ranges[1].past = 0;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 4: Validate
    num_groups = cre2_num_capturing_groups(re);
    computed_check = one_line_flag + num_groups + (int)ranges[0].start + (int)ranges[0].past + strings[0].length + strings[1].length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
    // API sequence test completed successfully
}