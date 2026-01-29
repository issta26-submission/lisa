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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)\\d*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "abc foo123 bar45 baz";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    const int nmatch = 3;
    cre2_string_t original;
    cre2_string_t quoted;
    char quoted_buf[128];
    int quote_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(&original, 0, sizeof(original));
    memset(&quoted, 0, sizeof(quoted));
    memset(quoted_buf, 0, sizeof(quoted_buf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    strings[0].data = text + 4; strings[0].length = 6;  // "foo123"
    strings[1].data = text + 11; strings[1].length = 5; // "bar45"
    strings[2].data = text + 17; strings[2].length = 3; // "baz"
    cre2_strings_to_ranges(text, ranges, strings, nmatch);
    original.data = pattern;
    original.length = pattern_len;
    quoted.data = quoted_buf;
    quoted.length = (int)(sizeof(quoted_buf) - 1);
    quote_ret = cre2_quote_meta(&quoted, &original);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)quote_ret
               + (long)strings[0].length + (long)strings[1].length + (long)strings[2].length
               + (long)ranges[0].start + (long)ranges[0].past + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}