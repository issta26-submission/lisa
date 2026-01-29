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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }

    // Enable Perl character classes on the options object.
    cre2_opt_set_perl_classes(opt, 1);

    // Query the one-line option (should be an int, typically 0 or 1).
    int one_line = cre2_opt_one_line(opt);
    (void)one_line; // silence unused-variable warnings if any

    // Create a simple pattern using the options and then delete the compiled regexp.
    const char pattern[] = "^(a)(b)c$";
    int pat_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2; // failed to compile pattern
    }

    // Clean up the compiled regexp and the options.
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}