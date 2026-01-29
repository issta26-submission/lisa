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
//<ID> 1905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(test)(expr)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    const char * re_pat = NULL;
    int add_res1 = 0;
    int add_res2 = 0;
    int compile_res = 0;
    int case_flag = 0;
    int never_nl_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_never_nl(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, "foo");
    add_res2 = cre2_set_add_simple(set, "bar");
    compile_res = cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    re_pat = cre2_pattern(re);
    case_flag = cre2_opt_case_sensitive(opt);
    never_nl_flag = cre2_opt_never_nl(opt);

    // step 4: Validate
    checksum = (long)pattern_len + (long)add_res1 + (long)add_res2 + (long)compile_res + (long)(re_pat != NULL) + (long)case_flag + (long)never_nl_flag + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}