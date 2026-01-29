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
//<ID> 1962
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char text[] = "foobar baz foo";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_string_t text_str = { text, text_len };
    const char rewrite_literal[] = "$1";
    cre2_string_t rewrite_str = { rewrite_literal, (int)(sizeof(rewrite_literal) - 1) };
    char target_buf[64] = { 0 };
    cre2_string_t target_str = { target_buf, (int)sizeof(target_buf) };
    cre2_string_t err_arg = { NULL, 0 };
    cre2_string_t matches[3] = { { text, 3 }, { text + 3, 3 }, { NULL, 0 } };
    cre2_range_t ranges[3] = { { 0, 0 }, { 0, 0 }, { 0, 0 } };
    int extract_ret = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    cre2_error_arg(re, &err_arg);
    cre2_strings_to_ranges(text, ranges, matches, 3);

    // step 4: Validate
    checksum = (long)pattern_len + (long)text_len + (long)extract_ret + (long)target_str.length + ranges[0].start + ranges[0].past + (long)(opt != NULL) + (long)(re != NULL) + (long)err_arg.length;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}