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
//<ID> 1336
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
    char text_buf[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_regexp_t *re = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    cre2_string_t min_str;
    cre2_string_t max_str;
    int pmr_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(&min_str, 0, sizeof(min_str));
    memset(&max_str, 0, sizeof(max_str));
    /* prepare strings that reference substrings inside text_buf:
       "foobar" begins at index 7 in "prefix foobar suffix" */
    strings[0].data = text_buf + 7; /* "foobar" -> "foo" at offset 7 */
    strings[0].length = 3;         /* "foo" */
    strings[1].data = text_buf + 10; /* "bar" starts at offset 10 */
    strings[1].length = 3;           /* "bar" */
    strings[2].data = text_buf;
    strings[2].length = text_len;

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, NULL);
    pmr_ret = cre2_possible_match_range(re, &min_str, &max_str, 32);
    cre2_strings_to_ranges(text_buf, ranges, strings, 3);

    // step 4: Validate (lightweight checksum using results)
    computed_check = (long)pattern_len + (long)text_len + (long)pmr_ret
                     + (long)(re != NULL) + (long)ranges[0].start + (long)ranges[0].past
                     + (long)min_str.length + (long)max_str.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // API sequence test completed successfully
    return 66;
}