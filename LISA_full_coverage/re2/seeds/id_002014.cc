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
//<ID> 2014
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)|(bar)[0-9]+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "bar123";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    int log_errors_flag = 0;
    int add_ret = 0;
    int compile_ret = 0;
    int match_slots[4];
    size_t matched_count = 0;
    long long checksum = 0;

    // step 2: Setup - create and configure options and set
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);
    set = cre2_set_new(opt, CRE2_ANCHOR_START);
    add_ret = cre2_set_add_simple(set, pattern);
    compile_ret = cre2_set_compile(set);

    // step 3: Operate - compile regexp, create named groups iterator, perform set match
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    memset(match_slots, 0, sizeof(match_slots));
    matched_count = cre2_set_match(set, text_buf, text_len, match_slots, 4);

    // step 4: Validate - compute a simple checksum using results
    checksum = (long long)pattern_len + (long long)log_errors_flag + (long long)compile_ret + (long long)add_ret + (long long)matched_count + (long long)match_slots[0] + (long long)(re != NULL) + (long long)(set != NULL) + (long long)(iter != NULL) + (long long)(pattern_from_re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}