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
//<ID> 1337
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
    cre2_regexp_t * re = NULL;
    cre2_string_t minstr;
    cre2_string_t maxstr;
    char minbuf[32];
    char maxbuf[32];
    int pmr_ret = 0;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    long computed_check = 0;

    // step 2: Setup
    memset(minbuf, 0, sizeof(minbuf));
    memset(maxbuf, 0, sizeof(maxbuf));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    /* prepare two substring descriptors that point into text_buf:
       "foo" starts at index 7, "bar" at index 10 in "prefix foobar suffix" */
    strings[0].data = text_buf + 7;
    strings[0].length = 3;
    strings[1].data = text_buf + 10;
    strings[1].length = 3;
    minstr.data = minbuf;
    minstr.length = (int)sizeof(minbuf);
    maxstr.data = maxbuf;
    maxstr.length = (int)sizeof(maxbuf);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, NULL);
    pmr_ret = cre2_possible_match_range(re, &minstr, &maxstr, 16);
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 4: Validate / lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)(re != NULL)
                     + (long)pmr_ret + (long)ranges[0].start + (long)ranges[0].past
                     + (long)ranges[1].start + (long)ranges[1].past
                     + (long)minstr.length + (long)maxstr.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}