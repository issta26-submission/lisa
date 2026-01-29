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
//<ID> 2039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)[0-9]+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "bar42";
    const size_t text_len = (size_t)(sizeof(text) - 1);
    cre2_options_t *opt = NULL;
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;
    int matches[4];
    size_t match_count = 0;
    int err_code = 0;
    int64_t desired_max_mem = (int64_t)16 * 1024 * 1024; // 16 MB
    long checksum = 0;

    // step 2: Setup - create options and configure max memory
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, desired_max_mem);

    // step 3: Operate - create a set, add pattern, compile, and also compile a regexp
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);
    err_code = cre2_error_code(re);
    memset(matches, 0, sizeof(matches));
    match_count = cre2_set_match(set, text, text_len, matches, 4);

    // step 4: Validate - compute a simple checksum to ensure values flow through
    checksum = (long)pattern_len + (long)err_code + (long)match_count + (long)matches[0] + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL) + (long)desired_max_mem;
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}