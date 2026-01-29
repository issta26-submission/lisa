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
//<ID> 583
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(fo+o)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int add_rc = 0;
    int compile_rc = 0;
    int errcode = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_rc = cre2_set_add_simple(set, pattern);
    compile_rc = cre2_set_compile(set);
    errcode = cre2_error_code(re);

    // step 5: Validate
    computed_check = (int)enc + add_rc + compile_rc + errcode + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}