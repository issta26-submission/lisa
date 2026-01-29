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
//<ID> 2222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
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
    int set_match_arr[8];
    size_t set_match_ret = 0;
    int cre2_match_ret = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&match, 0, sizeof(match));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Initialize regex, iterator and set
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &group_name, &group_index);
    cre2_named_groups_iter_delete(iter);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate
    match.data = text_buf;
    match.length = text_len;
    cre2_match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 3);
    set_match_ret = cre2_set_match(set, text_buf, (size_t)text_len, set_match_arr, (size_t)(sizeof(set_match_arr)/sizeof(set_match_arr[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)num_groups + (long)group_index + (long)(group_name != NULL)
             + (long)set_add_ret + (long)set_compile_ret + (long)cre2_match_ret + (long)set_match_ret + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}