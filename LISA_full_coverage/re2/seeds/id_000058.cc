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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a|b){1,3}(?:cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t min_range;
    cre2_string_t max_range;
    const char *version_str = NULL;
    cre2_encoding_t encoding_before = CRE2_UNKNOWN;
    cre2_encoding_t encoding_after = CRE2_UNKNOWN;
    const char *pattern_from_re = NULL;
    int possible_range_result = 0;

    // step 2: Setup
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    opt = cre2_opt_new();
    version_str = cre2_version_string();
    encoding_before = cre2_opt_encoding(opt);

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    encoding_after = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    possible_range_result = cre2_possible_match_range(re, &min_range, &max_range, 128);

    // step 5: Validate
    (void)version_str;
    (void)encoding_before;
    (void)encoding_after;
    (void)pattern_from_re;
    (void)possible_range_result;
    (void)min_range;
    (void)max_range;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}