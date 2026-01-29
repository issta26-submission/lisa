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
//<ID> 1423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char add_pattern[] = "ab";
    const char text[] = "ab abc a";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int match[8];
    const size_t match_len = 8;
    int ret_set_add = 0;
    int ret_set_compile = 0;
    size_t set_matches = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    memset(match, 0, sizeof(match));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    ret_set_add = cre2_set_add_simple(set, add_pattern);
    ret_set_compile = cre2_set_compile(set);
    set_matches = cre2_set_match(set, text, (size_t)text_len, match, match_len);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)num_groups + (long)ret_set_add
               + (long)ret_set_compile + (long)set_matches + (long)(re != NULL)
               + (long)(set != NULL) + (long)match[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}