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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(a+)(b*)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_range;
    cre2_string_t max_range;
    const char * re_pat = NULL;
    const char * ver = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int pmr_res = 0;
    int maxlen = 16;

    // step 2: Setup
    memset(&min_range, 0, sizeof(min_range));
    memset(&max_range, 0, sizeof(max_range));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    ver = cre2_version_string();
    re_pat = cre2_pattern(re);
    pmr_res = cre2_possible_match_range(re, &min_range, &max_range, maxlen);

    // step 5: Validate
    (void)enc;
    (void)ver;
    (void)re_pat;
    (void)pmr_res;
    (void)min_range.data;
    (void)min_range.length;
    (void)max_range.data;
    (void)max_range.length;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}