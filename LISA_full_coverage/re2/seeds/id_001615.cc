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
//<ID> 1615
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char text[] = "hello foo";
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int nmatch = 2;
    int one_line_flag = 0;
    int64_t max_mem = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)1048576); /* 1MB limit */
    one_line_flag = cre2_opt_one_line(opt);
    max_mem = cre2_opt_max_mem(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate
    strings[0].data = text;
    strings[0].length = 5;         /* "hello" */
    strings[1].data = text + 6;
    strings[1].length = 3;         /* "foo" */
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)one_line_flag + (long)max_mem
             + (long)ranges[0].start + (long)ranges[0].past
             + (long)ranges[1].start + (long)ranges[1].past
             + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}