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
//<ID> 2017
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
    const char set_pat1[] = "^foo";
    const char set_pat2[] = "bar$";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    char text_buf[] = "xxfoobarxx";
    const int text_len = (int)(sizeof(text_buf) - 1);
    int match_array[8];
    size_t set_match_ret = 0;
    long checksum = 0;
    int log_errors_flag = 0;

    // step 2: Setup - create and configure options, create and populate a set
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    (void)cre2_set_add_simple(set, set_pat1);
    (void)cre2_set_add_simple(set, set_pat2);
    (void)cre2_set_compile(set);

    // step 3: Operate - compile a regexp, create named-groups iterator, query pattern string, perform set match
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    pattern_from_re = cre2_pattern(re);
    memset(match_array, 0, sizeof(match_array));
    set_match_ret = cre2_set_match(set, text_buf, (size_t)text_len, match_array, (size_t)(sizeof(match_array) / sizeof(match_array[0])));

    // step 4: Validate - compute a checksum from results to ensure data flowed through APIs
    checksum = (long)pattern_len + (long)text_len + (long)log_errors_flag + (long)set_match_ret + (long)(pattern_from_re != NULL) + (long)(iter != NULL) + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}