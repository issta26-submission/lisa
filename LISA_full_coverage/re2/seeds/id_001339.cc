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
//<ID> 1339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?:foo|bar)(baz)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfoobazyybarbazzz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_str;
    cre2_string_t max_str;
    cre2_string_t strings[2];
    cre2_range_t ranges[2];
    int pmr_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&min_str, 0, sizeof(min_str));
    memset(&max_str, 0, sizeof(max_str));
    memset(strings, 0, sizeof(strings));
    memset(ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pmr_ret = cre2_possible_match_range(re, &min_str, &max_str, 64);
    strings[0] = min_str;
    strings[1] = max_str;
    cre2_strings_to_ranges(text_buf, ranges, strings, 2);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)text_len + (long)pmr_ret
                     + (long)(ranges[0].start) + (long)(ranges[0].past)
                     + (long)(ranges[1].start) + (long)(ranges[1].past)
                     + (long)(re != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}