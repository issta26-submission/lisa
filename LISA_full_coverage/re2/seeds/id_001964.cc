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
//<ID> 1964
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text[] = "abxab";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_fmt[] = "$1-$2";
    cre2_string_t text_str = { text, text_len };
    cre2_string_t rewrite_str = { rewrite_fmt, (int)(sizeof(rewrite_fmt) - 1) };
    cre2_string_t target = { NULL, 0 };
    cre2_string_t errarg = { NULL, 0 };
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    cre2_error_arg(re, &errarg);
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target);

    // step 4: Convert string slices to ranges
    strings[0].data = text + 0;
    strings[0].length = 1;
    strings[1].data = text + 1;
    strings[1].length = 1;
    memset(ranges, 0, sizeof(ranges));
    cre2_strings_to_ranges(text, ranges, strings, 2);

    // step 5: Validate (compute a checksum to use results meaningfully)
    checksum = (long)pattern_len + (long)text_len + (long)rewrite_str.length + (long)errarg.length + (long)extract_ret + (long)strings[0].length + (long)strings[1].length + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}