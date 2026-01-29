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
//<ID> 1618
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char text[] = "foobar baz";
    cre2_string_t strings[2] = {
        { text, 6 },        // "foobar" -> first 6 chars
        { text + 7, 3 }     // "baz" -> starting at index 7
    };
    cre2_range_t ranges[2] = { {0, 0}, {0, 0} };
    int nmatch = 2;
    long long checksum = 0;
    long long max_mem_val = 0;
    int one_line_val = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(1024LL * 1024LL));
    one_line_val = cre2_opt_one_line(opt);
    max_mem_val = (long long)cre2_opt_max_mem(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long long)pattern_len + (long long)one_line_val + max_mem_val
               + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past
               + (long long)(opt != NULL) + (long long)(re != NULL) + (long long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}