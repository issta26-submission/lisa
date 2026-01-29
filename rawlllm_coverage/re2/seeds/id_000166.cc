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
//<ID> 166
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

    // Enable "never capture" on the options
    cre2_opt_set_never_capture(opt, 1);

    // Query version interface to ensure API is reachable
    int ver = cre2_version_interface_current();
    if (ver < 0) {
        cre2_opt_delete(opt);
        return 2; // unexpected version value
    }

    // Test deleting a set pointer safely (use nullptr)
    cre2_set_delete(nullptr);

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}