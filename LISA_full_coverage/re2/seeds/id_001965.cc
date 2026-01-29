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
//<ID> 1965
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "foobarbaz foo";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[3];
    cre2_range_t ranges[3];
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t errarg;
    int easy_match_ret = 0;
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    memset(ranges, 0, sizeof(ranges));
    text_str.data = text;
    text_str.length = text_len;
    rewrite_str.data = "$1"; // request first capture in rewrite
    rewrite_str.length = (int)(sizeof("$1") - 1);
    target_str.data = NULL;
    target_str.length = 0;
    errarg.data = NULL;
    errarg.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    easy_match_ret = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    cre2_error_arg(re, &errarg);
    cre2_strings_to_ranges(text, ranges, matches, 3);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)easy_match_ret + (long)extract_ret + (long)target_str.length + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)(errarg.data != NULL) + (long)ranges[0].start + (long)ranges[0].past + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}