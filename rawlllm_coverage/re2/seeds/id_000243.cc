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
//<ID> 243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Allocate options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // Configure options: enable never-capture mode
    cre2_opt_set_never_capture(opt, 1);

    // Query a different option to exercise getter API
    int is_literal = cre2_opt_literal(opt);
    (void)is_literal; // value not used further, just exercise the API

    // Create a set using the options (anchor value 0)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set
    cre2_set_add_simple(set, "test.*");

    // Compile the set
    int compile_result = cre2_set_compile(set);
    (void)compile_result; // tolerate any numeric result; we exercised the API

    // Build a simple regexp using the same options to exercise cre2_error_string
    cre2_regexp_t *re = cre2_new("test", 4, opt);
    if (re) {
        // Retrieve any error string associated with this regexp (may be NULL or empty)
        const char *err = cre2_error_string(re);
        (void)err; // just exercise the API

        // Clean up the regexp
        cre2_delete(re);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Success
    return 66;
}