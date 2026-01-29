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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(?:a|ab|abc)+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_range;
    cre2_string_t max_range;
    cre2_string_t version_str;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * pattern_from_re = NULL;
    const char * version = NULL;
    int possible_res = 0;
    int maxlen = 32;

    // step 2: Setup
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    memset(&version_str, 0, sizeof(version_str));
    opt = cre2_opt_new();

    // step 3: Configure
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    possible_res = cre2_possible_match_range(re, &min_range, &max_range, maxlen);
    pattern_from_re = cre2_pattern(re);
    version = cre2_version_string();
    version_str.data = version;
    version_str.length = 0;

    // step 5: Validate
    (void)enc;
    (void)possible_res;
    (void)min_range;
    (void)max_range;
    (void)pattern_from_re;
    (void)version_str;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}