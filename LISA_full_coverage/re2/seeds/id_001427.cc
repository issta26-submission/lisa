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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char add_pattern[] = "foo";
    const char text[] = "foo baz foo";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    size_t match_count = 0;
    int matches[4];
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    set_add_ret = cre2_set_add_simple(set, add_pattern);
    set_compile_ret = cre2_set_compile(set);
    match_count = cre2_set_match(set, text, text_len, matches, 4);
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate / compute lightweight checksum
    checksum = (long)pattern_len + (long)text_len + (long)set_add_ret + (long)set_compile_ret
               + (long)match_count + (long)matches[0] + (long)num_groups + (long)(re != NULL)
               + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}