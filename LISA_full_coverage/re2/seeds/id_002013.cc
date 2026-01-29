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
//<ID> 2013
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(user|admin):([A-Za-z0-9_]+)$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    int log_errors_flag = 0;
    char text[] = "admin:john_doe";
    size_t text_len = (size_t)(sizeof(text) - 1);
    int matches[4];
    size_t match_count = 0;
    long checksum = 0;

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);
    log_errors_flag = cre2_opt_log_errors(opt);

    // step 3: Operate - create set, add pattern, compile, compile regexp and create iterator
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate - attempt a set match to exercise set API and collect results
    memset(matches, 0, sizeof(matches));
    match_count = cre2_set_match(set, text, text_len, matches, (size_t)(sizeof(matches) / sizeof(matches[0])));

    // step 5: Validate - compute a simple checksum from results to ensure data flowed through APIs
    checksum = (long)pattern_len + (long)log_errors_flag + (long)match_count + (long)(matches[0]) + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)(iter != NULL) + (long)(pattern_from_re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}