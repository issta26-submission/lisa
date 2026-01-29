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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable Perl character classes and verify via the getter
    cre2_opt_set_perl_classes(opt, 1);
    int perl_flag = cre2_opt_perl_classes(opt);
    if (!perl_flag) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Compile a simple valid pattern using the options
    const char *pattern = "a(b)c";
    int pattern_len = 5; // length of "a(b)c"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 3;
    }

    // Check for errors on the compiled regexp
    int err = cre2_error_code(re);
    if (err != 0) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 4;
    }

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}