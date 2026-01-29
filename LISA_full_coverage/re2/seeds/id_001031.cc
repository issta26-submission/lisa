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
//<ID> 1031
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo|bar|baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "this text mentions foo and also bar somewhere";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char min_buf[64];
    char max_buf[64];
    int match_indices[8];
    size_t match_slots = 8;
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t min_range;
    cre2_string_t max_range;
    long computed_check = 0;
    int add_res = 0;
    int compile_res = 0;
    size_t set_matches = 0;

    // step 2: Setup
    memset(min_buf, 0, sizeof(min_buf));
    memset(max_buf, 0, sizeof(max_buf));
    memset(match_indices, 0, sizeof(match_indices));
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Core operations - compile a regexp and add pattern to set
    re = cre2_new(pattern, pattern_len, opt);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate - compute possible match range for the compiled regexp
    min_range.data = min_buf;
    min_range.length = 0;
    max_range.data = max_buf;
    max_range.length = 0;
    (void)cre2_possible_match_range(re, &min_range, &max_range, (int)(sizeof(min_buf) - 1));

    // step 5: Operate - run set match on the text
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_indices, match_slots);

    // step 6: Validate / Cleanup
    computed_check = (long)set_matches + (long)match_indices[0] + (long)min_range.length + (long)max_range.length + (long)pattern_len + (long)add_res + (long)compile_res;
    (void)computed_check;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}