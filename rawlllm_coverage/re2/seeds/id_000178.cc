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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable Perl character classes.
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        // Failed to allocate options - treat as failure (non-66).
        return 1;
    }
    cre2_opt_set_perl_classes(opt, 1);

    // Create a set using the options (anchor value 0 used as a default).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set != nullptr) {
        // Add a simple pattern to the set and compile it.
        const char set_pattern[] = "foo|bar";
        (void)cre2_set_add_simple(set, set_pattern);
        (void)cre2_set_compile(set);

        // Delete the set when done.
        cre2_set_delete(set);
        set = nullptr;
    }

    // Prepare a regular expression using the same options.
    const char pattern[] = "(hello)\\s+(world)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (rex == nullptr) {
        // Clean up options and return failure code.
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text and match containers.
    const char input_text[] = "hello   world and more";
    cre2_string_t text;
    text.data = input_text;
    text.length = (int)(sizeof(input_text) - 1);

    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;

    // Attempt to find and consume using the compiled regexp.
    // Use nmatch = 2 to capture the whole match plus one capturing group.
    int nmatch = 2;
    int rc = cre2_find_and_consume_re(rex, &text, &match, nmatch);
    (void)rc; // In this test sequence we don't assert rc; just exercise the API.

    // Clean up regexp and options.
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // Indicate success.
    return 66;
}