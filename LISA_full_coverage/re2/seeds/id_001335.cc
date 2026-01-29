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
//<ID> 1335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_string_t min_s;
    cre2_string_t max_s;
    char min_buf[32];
    char max_buf[32];
    int possible_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    strings[0].data = text_buf + 7;   // points to "foo"
    strings[0].length = 3;
    strings[1].data = text_buf + 11;  // points to "bar"
    strings[1].length = 3;
    min_s.data = min_buf;
    min_s.length = (int)sizeof(min_buf);
    max_s.data = max_buf;
    max_s.length = (int)sizeof(max_buf);

    // step 3: Initialize / Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);
    possible_ret = cre2_possible_match_range(re, &min_s, &max_s, (int)sizeof(min_buf));

    // step 5: Validate
    computed_check = (long)pattern_len
                     + (long)text_len
                     + (long)strings[0].length
                     + (long)strings[1].length
                     + (long)ranges[0].start
                     + (long)ranges[0].past
                     + (long)ranges[1].start
                     + (long)ranges[1].past
                     + (long)(re != NULL)
                     + (long)possible_ret
                     + (long)min_s.length
                     + (long)max_s.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}