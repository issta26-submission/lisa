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
//<ID> 1469
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo and baz foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t original;
    cre2_string_t quoted;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int quote_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    original.data = text_buf;
    original.length = text_len;
    quoted.data = NULL;
    quoted.length = 0;
    quote_ret = cre2_quote_meta(&quoted, &original);
    strings[0] = original;
    strings[1] = quoted;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)quote_ret + (long)(re != NULL)
               + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}