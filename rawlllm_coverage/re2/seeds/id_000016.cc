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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable Perl character classes
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_perl_classes(opt, 1);

    // Create a regexp from a small pattern using the options
    const char *pattern = "^(\\w+)=(\\d+)$";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // We don't need the compiled regexp further for this sequence; delete it
    cre2_delete(re);

    // Create a set, add a simple pattern, and compile the set
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 3;
    }

    // Use the simple add helper; ignore return for fuzzing/demo purposes
    (void)cre2_set_add_simple(set, pattern);

    // Compile the set (required API usage)
    (void)cre2_set_compile(set);

    // Clean up
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}