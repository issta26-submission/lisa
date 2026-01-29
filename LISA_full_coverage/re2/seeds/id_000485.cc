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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)(baz)?$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t matches[4];
    int dot_nl_flag = 0;
    int version_rev = 0;
    int prog_size = 0;
    int num_captures = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);

    // step 3: Configure
    dot_nl_flag = cre2_opt_dot_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    prog_size = cre2_program_size(re);
    num_captures = cre2_num_capturing_groups(re);
    version_rev = cre2_version_interface_revision();

    // step 5: Validate
    computed_check = version_rev + dot_nl_flag + prog_size + num_captures;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}