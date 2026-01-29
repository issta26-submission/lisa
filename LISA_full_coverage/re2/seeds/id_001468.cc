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
//<ID> 1468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha)( beta)?( gamma)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alpha beta gamma";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t original;
    cre2_string_t quoted;
    char quote_buf[128];
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    const int nmatch = 3;
    int quote_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(quote_buf, 0, sizeof(quote_buf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    original.data = pattern;
    original.length = pattern_len;
    quoted.data = quote_buf;
    quoted.length = (int)(sizeof(quote_buf) - 1);
    quote_ret = cre2_quote_meta(&quoted, &original);
    strings[0].data = text_buf;
    strings[0].length = 5;        // "alpha"
    strings[1].data = text_buf + 6;
    strings[1].length = 4;        // "beta"
    strings[2].data = text_buf + 11;
    strings[2].length = 5;        // "gamma"
    cre2_strings_to_ranges(text_buf, ranges, strings, nmatch);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)quote_ret
               + (long)ranges[0].start + (long)ranges[0].past
               + (long)ranges[1].start + (long)ranges[1].past
               + (long)ranges[2].start + (long)ranges[2].past
               + (long)(re != NULL) + (long)(opt != NULL) + (long)quoted.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}