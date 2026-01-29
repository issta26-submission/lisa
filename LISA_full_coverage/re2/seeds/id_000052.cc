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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar).*(baz)?$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t min_range;
    cre2_string_t max_range;
    const char * pat_from_re = NULL;
    const char * version = NULL;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int possible_res = 0;
    int summary = 0;

    // step 2: Setup
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    encoding = cre2_opt_encoding(opt);

    // step 3: Configure
    (void)encoding;

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    possible_res = cre2_possible_match_range(re, &min_range, &max_range, 64);
    version = cre2_version_string();

    // step 5: Validate
    summary = possible_res + (int)encoding + (int)(pat_from_re != NULL) + (int)(version != NULL) + min_range.length + max_range.length;
    (void)summary;
    (void)pat_from_re;
    (void)version;
    (void)min_range;
    (void)max_range;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}