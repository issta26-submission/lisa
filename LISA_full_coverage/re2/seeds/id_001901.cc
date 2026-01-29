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
//<ID> 1901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    const char * re_pat = NULL;
    int add_res = 0;
    int compile_res = 0;
    int num_groups = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_never_nl(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, "hello");
    compile_res = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    re_pat = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);
    err_code = cre2_error_code(re);

    // step 4: Validate
    checksum = (long)pattern_len + (long)(re_pat != NULL) + (long)add_res + (long)compile_res + (long)num_groups + (long)err_code + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}