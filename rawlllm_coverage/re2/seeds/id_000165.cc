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
//<ID> 165
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

    // Enable Perl character classes on the options and verify settings
    cre2_opt_set_perl_classes(opt, 1);

    // Query the maximum memory setting (use int64_t as specified by the API)
    int64_t max_mem = cre2_opt_max_mem(opt);

    // Create a cre2_set using the options and an unanchored search anchor
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2; // failed to create set
    }

    // Basic sanity check on returned max_mem to avoid unused-variable warning
    if (max_mem < 0) {
        // cleanup before returning
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Clean up resources
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}