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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set the word boundary option (required API usage)
    cre2_opt_set_word_boundary(opt, 1);

    // Query encoding from options (required API usage)
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; // silence unused-variable warnings if any

    // Prepare a pattern containing named groups so the iterator can find them
    const char *pattern = "(?P<first>hello)(?P<second>world)";
    int pat_len = 0;
    while (pattern[pat_len]) ++pat_len;

    // Compile the regexp using the options
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);

    // Delete the options (required API usage). cre2_new copies needed configuration.
    cre2_opt_delete(opt);
    opt = nullptr;

    if (!re) {
        // compilation failed
        return 2;
    }

    // Create a named-groups iterator and iterate over named groups
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = nullptr;
        int index = 0;
        // Use the iterator next function (required API usage)
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // Defensive checks; no external side effects required for this test
            if (!name) {
                // continue if name unexpectedly null
                continue;
            }
            // We could inspect 'name' and 'index' here; keep minimal for fuzzing harness
            (void)name;
            (void)index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Clean up the compiled regexp
    cre2_delete(re);

    return 66;
}