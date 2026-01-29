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
//<ID> 1634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * compiled_pat = NULL;
    int errcode = 0;
    int status_compile = 0;
    int nmatch_arr[8];
    size_t nmatched = 0;
    int ncap = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    status_compile = cre2_set_compile(set);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    errcode = cre2_error_code(re);
    ncap = cre2_num_capturing_groups(re);
    memset(nmatch_arr, -1, sizeof(nmatch_arr));
    nmatched = cre2_set_match(set, text, (size_t)text_len, nmatch_arr, (size_t)(sizeof(nmatch_arr) / sizeof(nmatch_arr[0])));

    // step 5: Validate
    checksum = (long)pattern_len + (long)status_compile + (long)errcode + (long)ncap + (long)nmatched
               + (long)(compiled_pat != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL)
               + (long)nmatch_arr[0] + (long)nmatch_arr[1];
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}