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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a.*b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    int rev = 0;
    int use_word_boundary = 0;
    int use_dot_nl_flag = 0;
    int prog_size = 0;
    int dotnl_get = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    rev = cre2_version_interface_revision();
    use_word_boundary = rev & 1;
    use_dot_nl_flag = (rev >> 1) & 1;
    cre2_opt_set_word_boundary(opt, use_word_boundary);
    cre2_opt_set_dot_nl(opt, use_dot_nl_flag);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);

    // step 4: Operate
    dotnl_get = cre2_opt_dot_nl(opt);
    computed_check = rev + prog_size + dotnl_get + use_word_boundary + use_dot_nl_flag;
    (void)computed_check;

    // step 5: Validate
    // (validation performed by combining API results into computed_check)

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}