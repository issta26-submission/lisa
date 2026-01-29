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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = cre2_opt_new();
    cre2_regexp_t * re = NULL;
    cre2_string_t min_str;
    cre2_string_t max_str;
    const char * pat_from_re = NULL;
    const char * version = NULL;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int range_res = 0;

    // step 2: Setup
    memset(&min_str, 0, sizeof(min_str));
    memset(&max_str, 0, sizeof(max_str));

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    range_res = cre2_possible_match_range(re, &min_str, &max_str, 64);
    encoding = cre2_opt_encoding(opt);
    version = cre2_version_string();

    // step 5: Validate
    (void)range_res;
    (void)pat_from_re;
    (void)min_str;
    (void)max_str;
    (void)encoding;
    (void)version;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}