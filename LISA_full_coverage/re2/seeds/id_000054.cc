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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a|b){1,3}x";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_range;
    cre2_string_t max_range;
    int maxlen = 64;
    int range_res = 0;
    const char * pattern_back = NULL;
    const char * ver = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;

    // step 2: Setup
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_back = cre2_pattern(re);
    ver = cre2_version_string();
    range_res = cre2_possible_match_range(re, &min_range, &max_range, maxlen);

    // step 5: Validate
    (void)enc;
    (void)pattern_back;
    (void)ver;
    (void)range_res;
    (void)min_range;
    (void)max_range;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}