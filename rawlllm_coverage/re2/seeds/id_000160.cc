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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for the set and for compiling the regexp
    cre2_options_t *opt = cre2_opt_new();
    cre2_set *set = NULL;
    cre2_regexp_t *re = NULL;

    // Create a set with the options (fall back to NULL if opt creation failed)
    set = cre2_set_new(opt ? opt : NULL, CRE2_UNANCHORED);
    if (set) {
        // Add a couple of simple patterns to the set
        cre2_set_add_simple(set, "hello");
        cre2_set_add_simple(set, "world");
    }

    // Compile a simple regexp using the same options (or NULL)
    const char pattern[] = "hello";
    re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt ? opt : NULL);

    // Prepare input string and match holder for consume
    const char input_text[] = "hello world";
    cre2_string_t input = { input_text, (int)(sizeof(input_text) - 1) };
    cre2_string_t match = { NULL, 0 };

    if (re) {
        // Attempt to consume from the input using the compiled regexp
        (void)cre2_consume_re(re, &input, &match, 0);
    }

    // Demonstrate quoting meta characters from an original string
    const char orig_text[] = "a+b?^.";
    cre2_string_t original = { orig_text, (int)(sizeof(orig_text) - 1) };
    cre2_string_t quoted = { NULL, 0 };
    (void)cre2_quote_meta(&quoted, &original);

    // Clean up resources
    if (re) {
        cre2_delete(re);
        re = NULL;
    }
    if (set) {
        cre2_set_delete(set);
        set = NULL;
    }
    if (opt) {
        cre2_opt_delete(opt);
        opt = NULL;
    }

    // Return the required success code
    return 66;
}