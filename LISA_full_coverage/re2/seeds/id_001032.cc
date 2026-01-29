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
//<ID> 1032
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)\\d+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo123 and bar456 then foo789 end";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    char min_buf[64];
    char max_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t min_str;
    cre2_string_t max_str;
    int match_indices[8];
    size_t set_matches = 0;
    int range_res = 0;
    long computed_check = 0;

    // step 2: Setup (create options and compile regexp)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate - compute possible match range for the compiled regexp
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    min_str.data = min_buf;
    min_str.length = 0;
    max_str.data = max_buf;
    max_str.length = 0;
    range_res = cre2_possible_match_range(re, &min_str, &max_str, 32);

    // step 4: Operate - create a set, add the pattern, compile the set and run a set match
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    memset(match_indices, -1, sizeof(match_indices));
    set_matches = cre2_set_match(set, text_buf, text_len, match_indices, sizeof(match_indices) / sizeof(match_indices[0]));

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)range_res + (long)set_matches + (long)min_str.length + (long)max_str.length + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}