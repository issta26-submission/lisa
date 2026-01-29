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
//<ID> 1969
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "helloworld hello";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite;
    cre2_string_t target;
    cre2_string_t errarg;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    memset(&errarg, 0, sizeof(errarg));
    memset(&target, 0, sizeof(target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_str.data = text;
    text_str.length = text_len;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    cre2_error_arg(re, &errarg);
    rewrite.data = "$1";
    rewrite.length = 2;
    extract_ret = cre2_extract_re(re, &text_str, &rewrite, &target);
    strings[0].data = text;
    strings[0].length = 5;           // "hello" at start
    strings[1].data = text + 5;
    strings[1].length = 5;           // "world" following
    strings[2].data = text + 11;
    strings[2].length = 5;           // second "hello"
    cre2_strings_to_ranges(text, ranges, strings, 3);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)extract_ret + (long)errarg.length + (long)target.length + ranges[0].start + ranges[0].past + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}