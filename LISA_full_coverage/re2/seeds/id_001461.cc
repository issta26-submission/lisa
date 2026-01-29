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
//<ID> 1461
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)c+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char quoted_buf[64];
    cre2_string_t original;
    cre2_string_t quoted;
    const char text_buf[] = "prefix abc middle abc suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const int nmatch = 2;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int quote_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(&original, 0, sizeof(original));
    memset(&quoted, 0, sizeof(quoted));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    original.data = pattern;
    original.length = pattern_len;
    quoted.data = quoted_buf;
    quoted.length = (int)(sizeof(quoted_buf));
    quote_ret = cre2_quote_meta(&quoted, &original);
    re = cre2_new(original.data, original.length, opt);
    strings[0].data = text_buf + 7;   // points to "abc" at first occurrence
    strings[0].length = 3;
    strings[1].data = text_buf + 19;  // points to "abc" at second occurrence
    strings[1].length = 3;
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)quote_ret + (long)(re != NULL)
               + (long)strings[0].length + (long)strings[1].length + (long)ranges[0].start + (long)ranges[1].start;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}