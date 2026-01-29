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
//<ID> 1465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a+)(b*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text[] = "abc123def";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t original;
    char orig_buf[] = "a+b?";
    cre2_string_t quoted;
    char quoted_buf[64];
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    const int nmatch = 3;
    long checksum = 0;

    // step 2: Setup
    memset(ranges, 0, sizeof(ranges));
    memset(strings, 0, sizeof(strings));
    memset(quoted_buf, 0, sizeof(quoted_buf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    original.data = orig_buf;
    original.length = (int)(sizeof(orig_buf) - 1);
    quoted.data = quoted_buf;
    quoted.length = 0;
    cre2_quote_meta(&quoted, &original);
    strings[0].data = text;
    strings[0].length = 3;
    strings[1].data = text + 3;
    strings[1].length = 3;
    strings[2].data = text + 6;
    strings[2].length = 3;
    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)original.length + (long)quoted.length
               + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past
               + (long)ranges[2].start + (long)ranges[2].past + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}