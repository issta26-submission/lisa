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
//<ID> 1610
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?(baz)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xxx abc def ghi yyy";
    const int nmatch = 3;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int one_line_flag = 0;
    int64_t max_mem = 0;
    const char * compiled_pat = NULL;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    max_mem = cre2_opt_max_mem(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    compiled_pat = cre2_pattern(re);

    // step 4: Operate
    memset(ranges, 0, sizeof(ranges));
    strings[0].data = text + 4; strings[0].length = 3;  // "abc"
    strings[1].data = text + 8; strings[1].length = 3;  // "def"
    strings[2].data = text + 12; strings[2].length = 3; // "ghi"
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)one_line_flag + (long)max_mem
             + (long)(re != NULL) + (long)(iter != NULL) + (long)(compiled_pat != NULL)
             + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past + ranges[2].start + ranges[2].past;
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}