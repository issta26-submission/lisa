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
//<ID> 1966
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text_buf[] = "ab ab a";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_string_t text = { text_buf, text_len };
    cre2_string_t rewrite = { "$1-$2", 5 };
    cre2_string_t target = { NULL, 0 };
    cre2_string_t errarg = { NULL, 0 };
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    cre2_error_arg(re, &errarg);
    extract_ret = cre2_extract_re(re, &text, &rewrite, &target);
    strings[0] = text;
    strings[1] = target;
    strings[2] = rewrite;
    cre2_strings_to_ranges(text_buf, ranges, strings, 3);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text.length + (long)rewrite.length + (long)extract_ret + (long)target.length + (long)errarg.length + (long)(ranges[0].start) + (long)(ranges[0].past) + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}