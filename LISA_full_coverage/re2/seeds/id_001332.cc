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
//<ID> 1332
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo|bar(baz)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    cre2_string_t strs[3];
    cre2_range_t ranges[3];
    cre2_string_t min_s;
    cre2_string_t max_s;
    char min_buf[64];
    char max_buf[64];
    int pmr_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(strs, 0, sizeof(strs));
    memset(ranges, 0, sizeof(ranges));
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    min_s.data = min_buf;
    min_s.length = 0;
    max_s.data = max_buf;
    max_s.length = 0;
    strs[0].data = text_buf + 7; strs[0].length = 3;   // "foo"
    strs[1].data = text_buf + 11; strs[1].length = 3;  // "bar"
    strs[2].data = text_buf; strs[2].length = text_len;

    // step 3: Configure
    // (no cre2_options_t used here; pattern compiled with default options)

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, NULL);
    cre2_strings_to_ranges(text_buf, ranges, strs, 3);
    pmr_ret = cre2_possible_match_range(re, &min_s, &max_s, 32);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)pmr_ret
                     + (long)ranges[0].start + (long)ranges[0].past
                     + (long)min_s.length + (long)max_s.length + (long)(re != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}