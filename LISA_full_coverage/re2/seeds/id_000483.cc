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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar).+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    int rev = 0;
    int prev_dot_nl = 0;
    int word_flag = 0;
    int prog_size = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    rev = cre2_version_interface_revision();
    prev_dot_nl = cre2_opt_dot_nl(opt);
    word_flag = (rev ^ prev_dot_nl) & 1;
    cre2_opt_set_word_boundary(opt, word_flag);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    prog_size = cre2_program_size(re);

    // step 5: Validate
    computed_check = rev + prev_dot_nl + word_flag + prog_size + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}