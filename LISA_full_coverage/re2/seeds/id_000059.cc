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
//<ID> 59
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
    cre2_string_t min_str;
    cre2_string_t max_str;
    int possible_range_res = 0;
    int maxlen = 64;
    const char * extracted_pattern = NULL;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    const char * ver = NULL;

    // step 2: Setup
    memset(&min_str, 0, sizeof(min_str));
    memset(&max_str, 0, sizeof(max_str));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    extracted_pattern = cre2_pattern(re);
    possible_range_res = cre2_possible_match_range(re, &min_str, &max_str, maxlen);
    encoding = cre2_opt_encoding(opt);
    ver = cre2_version_string();

    // step 5: Validate
    (void)possible_range_res;
    (void)extracted_pattern;
    (void)min_str;
    (void)max_str;
    (void)encoding;
    (void)ver;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}