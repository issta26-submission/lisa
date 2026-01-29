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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options allocation fails, nothing to delete; still treat as failure to exercise API.
        return 66;
    }

    // Set never-capture to true, then later toggle it back.
    cre2_opt_set_never_capture(opt, 1);

    // Query the literal flag (read-only getter).
    int lit_before = cre2_opt_literal(opt);

    // Query encoding.
    cre2_encoding_t enc = cre2_opt_encoding(opt);

    // Call version interface revision to exercise version API.
    int rev = cre2_version_interface_revision();

    // Toggle never-capture back to false and re-check literal.
    cre2_opt_set_never_capture(opt, 0);
    int lit_after = cre2_opt_literal(opt);

    // Silence unused-variable warnings in builds with strict warnings.
    (void)lit_before;
    (void)lit_after;
    (void)enc;
    (void)rev;

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}