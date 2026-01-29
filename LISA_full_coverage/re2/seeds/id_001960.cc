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
//<ID> 1960
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "abc abc";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_template[] = "$1-$2";
    const int rewrite_len = (int)(sizeof(rewrite_template) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str = { text, text_len };
    cre2_string_t rewrite_str = { rewrite_template, rewrite_len };
    cre2_string_t target_str = { NULL, 0 };
    cre2_string_t arg_str = { NULL, 0 };
    cre2_string_t strings[3] = { {NULL, 0}, {NULL, 0}, {NULL, 0} };
    cre2_range_t ranges[3] = { {0, 0}, {0, 0}, {0, 0} };
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    // step 4: Conversion and error inspection
    strings[0] = text_str;
    strings[1] = rewrite_str;
    strings[2] = target_str;
    cre2_strings_to_ranges(text, ranges, strings, 3);
    cre2_error_arg(re, &arg_str);

    // step 5: Validate (compute a checksum to use results meaningfully)
    checksum = (long)pattern_len + (long)text_len + (long)rewrite_len + (long)extract_ret
               + (long)strings[0].length + (long)strings[1].length + (long)strings[2].length
               + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past
               + (long)ranges[2].start + (long)ranges[2].past + (long)(arg_str.data != NULL)
               + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}