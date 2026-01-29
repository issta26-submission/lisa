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
//<ID> 1429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "((foo)bar)?(baz)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "xyz foobar baz qux";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int set_match_results[8];
    const size_t set_match_len = sizeof(set_match_results) / sizeof(set_match_results[0]);
    int ret_add = 0;
    int ret_compile = 0;
    size_t matched_count = 0;
    int num_caps = 0;
    long checksum = 0;

    // step 2: Setup
    memset(set_match_results, 0, sizeof(set_match_results));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    ret_add = cre2_set_add_simple(set, "foobar");
    ret_compile = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    num_caps = cre2_num_capturing_groups(re);
    matched_count = cre2_set_match(set, text, text_len, set_match_results, set_match_len);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)ret_add + (long)ret_compile
               + (long)matched_count + (long)num_caps + (long)(set != NULL) + (long)(opt != NULL)
               + (long)set_match_results[0] + (long)set_match_results[1];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}