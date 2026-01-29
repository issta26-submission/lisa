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
//<ID> 1035
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo xyz bar foo123 bar";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    char min_buf[64];
    char max_buf[64];
    cre2_string_t min_s;
    cre2_string_t max_s;
    int possible_res = 0;
    int match_indices[8];
    size_t set_match_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    min_s.data = min_buf;
    min_s.length = 0;
    max_s.data = max_buf;
    max_s.length = 0;
    memset(match_indices, 0, sizeof(match_indices));

    // step 3: Operate - compile single regexp and compute possible match range
    re = cre2_new(pattern, pattern_len, opt);
    possible_res = cre2_possible_match_range(re, &min_s, &max_s, 16);

    // step 4: Operate - run set matching against the text buffer
    set_match_res = cre2_set_match(set, text_buf, text_len, match_indices, 8);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)possible_res + (long)min_s.length + (long)max_s.length + (long)set_match_res + (long)match_indices[0] + (long)pattern_len + (long)text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}