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
//<ID> 1334
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
    char text_buf[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t * re = NULL;
    cre2_string_t min_s;
    cre2_string_t max_s;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int ret_possible = 0;
    long computed_check = 0;
    char scratch[32];

    // step 2: Setup
    memset(&min_s, 0, sizeof(min_s));
    memset(&max_s, 0, sizeof(max_s));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(scratch, 0, sizeof(scratch));
    strings[0].data = text_buf + 7; strings[0].length = 3;   // "foo"
    strings[1].data = text_buf + 10; strings[1].length = 3;  // "bar"
    strings[2].data = NULL; strings[2].length = 0;

    // step 3: Core operations (Initialize -> Operate)
    re = cre2_new(pattern, pattern_len, NULL);
    ret_possible = cre2_possible_match_range(re, &min_s, &max_s, 16);

    // step 4: Further operations (Convert strings to ranges)
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)ret_possible
                     + (long)min_s.length + (long)max_s.length
                     + (long)ranges[0].start + (long)ranges[0].past
                     + (long)strings[0].length + (long)scratch[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}