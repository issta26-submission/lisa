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
//<ID> 808
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<g1>a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxabcdeyy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const int nmatch = 3;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int one_line_flag = 0;
    int computed_check = 0;

    // step 2: Setup / Configure
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    one_line_flag = cre2_opt_one_line(opt);

    // step 3: Operate (compile regex, create iterator, convert strings to ranges)
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    strings[0].data = text_buf + 2; strings[0].length = 1;   // "a"
    strings[1].data = text_buf + 3; strings[1].length = 1;   // "b"
    strings[2].data = text_buf + 4; strings[2].length = 2;   // "cd"
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 4: Validate (compute a checksum from results to ensure data flow)
    computed_check = one_line_flag
                     + strings[0].length + strings[1].length + strings[2].length
                     + (int)ranges[0].start + (int)ranges[0].past
                     + (int)(re != NULL) + (int)(iter != NULL);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}