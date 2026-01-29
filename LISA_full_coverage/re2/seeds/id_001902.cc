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
//<ID> 1902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(one)(two)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int add1_res = 0;
    int add2_res = 0;
    int compile_res = 0;
    const char * re_pat = NULL;
    int num_groups = 0;
    int err_code = 0;
    char scratch[16];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_never_nl(opt, 1);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add1_res = cre2_set_add_simple(set, "one");
    add2_res = cre2_set_add_simple(set, "two");
    compile_res = cre2_set_compile(set);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);
    err_code = cre2_error_code(re);

    // step 4: Validate
    memset(scratch, 0, sizeof(scratch));
    scratch[0] = re_pat ? re_pat[0] : 0;
    checksum = (long)pattern_len + (long)add1_res + (long)add2_res + (long)compile_res + (long)(re_pat != NULL) + (long)num_groups + (long)err_code + (long)scratch[0];
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}