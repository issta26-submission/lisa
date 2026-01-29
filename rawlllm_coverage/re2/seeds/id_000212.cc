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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();

    // Query current library interface version
    int ver = cre2_version_interface_current();
    (void)ver; // silence unused-warning if any

    // If options were created, query and toggle the 'perl classes' flag
    if (opt) {
        int perl_before = cre2_opt_perl_classes(opt);
        // Toggle the flag
        cre2_opt_set_perl_classes(opt, perl_before ? 0 : 1);
        int perl_after = cre2_opt_perl_classes(opt);
        (void)perl_before;
        (void)perl_after;

        // Clean up options
        cre2_opt_delete(opt);
    }

    // Exercise cre2_delete with a null pointer (safe if implementation allows NULL)
    cre2_regexp_t *re = nullptr;
    cre2_delete(re);

    return 66;
}