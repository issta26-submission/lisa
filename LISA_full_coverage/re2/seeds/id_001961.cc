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
//<ID> 1961
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "foobarXYZ";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t rewrite = { "$1-$2", 5 };
    cre2_string_t target = { NULL, 0 };
    cre2_string_t errarg = { NULL, 0 };
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    extract_ret = cre2_extract_re(re, &text, &rewrite, &target);
    cre2_error_arg(re, &errarg);

    // prepare strings that point into the original text buffer and convert to ranges
    strings[0].data = text_buf + 0;
    strings[0].length = 3; // "foo"
    strings[1].data = text_buf + 3;
    strings[1].length = 3; // "bar"
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)extract_ret + (long)target.length + (long)errarg.length + (long)strings[0].length + (long)strings[1].length + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}