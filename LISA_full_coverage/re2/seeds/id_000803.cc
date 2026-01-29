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
//<ID> 803
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabczz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_range_t ranges[2];
    cre2_string_t strings[2];
    int nmatch = 2;
    int one_line_flag = 0;
    int computed_check = 0;

    // step 2: Setup / Configure
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_one_line(opt, 0);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Operate - compile regexp and create named group iterator
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate - prepare strings and convert to ranges
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text_buf + 2;
    strings[0].length = 3; /* "abc" */
    strings[1].data = text_buf + 5;
    strings[1].length = 2; /* "zz" */
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);
    one_line_flag = cre2_opt_one_line(opt);

    // step 5: Validate
    computed_check = (int)ranges[0].start + (int)ranges[0].past + strings[0].length + one_line_flag + (re != NULL) + (iter != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_named_groups_iter_delete(iter);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}