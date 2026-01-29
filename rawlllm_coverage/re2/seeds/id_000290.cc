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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Configure options using the required APIs */
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);

    /* Create a simple pattern and compile it with the options */
    const char *pattern = "a.*b";
    int pattern_len = 4; /* number of bytes in "a.*b" (excluding terminating NUL) */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Clean up the compiled regexp (uses required cre2_delete) */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}