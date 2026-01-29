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
//<ID> 1968
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ex)(tract)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "extract this: extract";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text_str = { text_buf, text_len };
    const char rewrite_buf[] = "[$1:$2]";
    cre2_string_t rewrite_str = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    cre2_string_t target_str = { NULL, 0 };
    cre2_string_t err_arg = { NULL, 0 };
    cre2_range_t ranges[2];
    cre2_string_t strings_for_ranges[2];
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    cre2_error_arg(re, &err_arg);
    memset(ranges, 0, sizeof(ranges));
    strings_for_ranges[0] = rewrite_str;
    strings_for_ranges[1] = target_str;
    cre2_strings_to_ranges(text_buf, ranges, strings_for_ranges, 2);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)extract_ret + (long)rewrite_str.length + (long)target_str.length + (long)err_arg.length + (long)(re != NULL) + (long)(opt != NULL) + ranges[0].start + ranges[0].past + ranges[1].start + ranges[1].past;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}