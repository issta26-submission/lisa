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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a|b){1,3}c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_str;
    cre2_string_t max_str;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * re_pat = NULL;
    const char * ver = NULL;
    int pmr_res = 0;
    int maxlen = 64;

    // step 2: Setup
    memset(&min_str, 0, sizeof(min_str));
    memset(&max_str, 0, sizeof(max_str));
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);
    maxlen += (int)enc;

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    ver = cre2_version_string();
    pmr_res = cre2_possible_match_range(re, &min_str, &max_str, maxlen);

    // step 4: Validate
    (void)re_pat;
    (void)ver;
    (void)pmr_res;
    (void)min_str;
    (void)max_str;
    (void)enc;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}