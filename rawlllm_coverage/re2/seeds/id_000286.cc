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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Allocate options and configure them.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable logging of errors.
    cre2_opt_set_log_errors(opt, 1);

    // Query POSIX syntax flag (use as a read; ignore value).
    int posix_flag = cre2_opt_posix_syntax(opt);
    (void)posix_flag;

    // Create a set with the provided options.
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set) {
        // Add a simple pattern to the set.
        cre2_set_add_simple(set, "abc");
    }

    // Prepare input text as a cre2_string_t.
    cre2_string_t text;
    text.data = "abc";
    text.length = 3;

    // Prepare output match array (space for full match + one capturing group).
    cre2_string_t matches[2];
    matches[0].data = nullptr; matches[0].length = 0;
    matches[1].data = nullptr; matches[1].length = 0;

    // Attempt a partial match using a pattern with one capturing group.
    int partial_ok = cre2_partial_match("a(b)c", &text, matches, 2);
    (void)partial_ok; // result can be inspected if desired

    // Clean up resources.
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}