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
//<ID> 176
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

    // Enable Perl character classes via setter, then verify with the getter.
    cre2_opt_set_perl_classes(opt, 1);
    int perl_classes = cre2_opt_perl_classes(opt);
    if (perl_classes != 1) {
        cre2_opt_delete(opt);
        return 2; // getter didn't reflect the set value
    }

    // Create a set with the options and an unanchored matching mode.
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 3; // failed to create set
    }

    // Add a couple of simple patterns to the set (ignore add return specifics).
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar[0-9]+");

    // Compile the set and ensure compilation succeeds (non-zero expected for success).
    int compile_ok = cre2_set_compile(set);
    if (!compile_ok) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 4; // compilation failed
    }

    // Clean up resources.
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}