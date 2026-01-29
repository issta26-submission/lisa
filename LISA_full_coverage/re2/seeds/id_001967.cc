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
//<ID> 1967
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "foo123 bar456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text_str = { text_buf, text_len };
    cre2_string_t rewrite = { "$1-$2", 5 };
    cre2_string_t target = { NULL, 0 };
    cre2_string_t err_arg = { NULL, 0 };
    cre2_string_t strings[2] = { {NULL, 0}, {NULL, 0} };
    cre2_range_t ranges[2] = { {0, 0}, {0, 0} };
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    extract_ret = cre2_extract_re(re, &text_str, &rewrite, &target);

    // step 4: Inspect/collect error argument and prepare strings for range conversion
    cre2_error_arg(re, &err_arg);
    strings[0] = rewrite;
    strings[1] = target;

    // step 5: Convert strings to ranges within the original text
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 6: Validate and Cleanup
    checksum = (long)pattern_len + (long)text_len + (long)extract_ret + (long)rewrite.length + (long)target.length + (long)err_arg.length + (long)ranges[0].start + (long)ranges[0].past + (long)ranges[1].start + (long)ranges[1].past + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}