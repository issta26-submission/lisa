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
//<ID> 351
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Query the library interface version
    int ver = cre2_version_interface_current();
    if (ver < 0) {
        // unexpected version value
        return 1;
    }

    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 2;
    }

    // Configure options: disable capturing
    cre2_opt_set_never_capture(opt, 1);

    // Inspect a boolean option (word boundary) to exercise the getter
    int wb = cre2_opt_word_boundary(opt);
    (void)wb; // silence unused variable warning in case value is unused

    // Prepare mutable text buffer and rewrite buffer for global replace.
    // Use stack arrays so they are safely writable if the API expects to modify them.
    char text_buf[] = "the cat sat on the catalyst catalog";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

    char rewrite_buf[] = "dog";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    // Pattern: whole-word "cat"
    const char *pattern = "\\bcat\\b";

    // Perform a global replace using the pattern and the rewrite string
    int replace_res = cre2_global_replace(pattern, &text_and_target, &rewrite);
    if (replace_res < 0) {
        cre2_opt_delete(opt);
        return 3;
    }

    // Clean up options
    cre2_opt_delete(opt);

    // Success
    return 66;
}