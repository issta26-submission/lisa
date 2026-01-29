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
//<ID> 2227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>[A-Za-z]+)-(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456 Carol-789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_named_groups_iter_t * iter = NULL;
    const char * group_name = NULL;
    int group_index = 0;
    struct cre2_set * set = NULL;
    cre2_string_t match;
    int num_groups = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int match_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Initialize regex and query group count
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Named groups iterator usage
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &group_name, &group_index);
    cre2_named_groups_iter_delete(iter);

    // step 5: Create and populate a set, then perform a match
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 3);

    // step 6: Validate and Cleanup
    checksum = (long)pattern_len + (long)text_len + (long)num_groups + (long)group_index + (long)(group_name != NULL)
             + (long)set_add_ret + (long)set_compile_ret + (long)match_ret + (long)match.length + (long)(match.data != NULL)
             + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}