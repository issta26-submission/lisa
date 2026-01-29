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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar).*";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    int rev = 0;
    int dotnl_before = 0;
    int dotnl_after = 0;
    int prog_sz = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    dotnl_before = cre2_opt_dot_nl(opt);
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);
    dotnl_after = cre2_opt_dot_nl(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    prog_sz = cre2_program_size(re);
    rev = cre2_version_interface_revision();

    // step 5: Validate
    computed_check = rev + dotnl_before + dotnl_after + prog_sz;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}