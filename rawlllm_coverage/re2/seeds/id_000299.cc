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
//<ID> 299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    const char *pattern = "(ab)(cd)";
    int pattern_len = 8; /* length of "(ab)(cd)" */

    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    /* Set some options to exercise the API */
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_never_nl(opt, 1);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    int groups = cre2_num_capturing_groups(re);
    (void)groups; /* use the value to avoid unused-variable warnings */

    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}