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
//<ID> 1033
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "ab+c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx abc abbc aabbbc xyz abbc";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int match_array[8];
    size_t matches_found = 0;
    int possible_range_res = 0;
    char min_buf[64];
    char max_buf[64];
    cre2_string_t min_s;
    cre2_string_t max_s;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    memset(match_array, 0, sizeof(match_array));
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));

    // step 3: Core operations - compile a regexp and get possible match range
    re = cre2_new(pattern, pattern_len, opt);
    min_s.data = min_buf;
    min_s.length = 0;
    max_s.data = max_buf;
    max_s.length = 0;
    possible_range_res = cre2_possible_match_range(re, &min_s, &max_s, 16);

    // step 4: Core operations - create a set, add patterns, compile and match against text
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "abc");
    cre2_set_add_simple(set, "abbc");
    cre2_set_compile(set);
    matches_found = cre2_set_match(set, text_buf, text_len, match_array, 8);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)matches_found + (long)possible_range_res + (long)min_s.length + (long)max_s.length + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}