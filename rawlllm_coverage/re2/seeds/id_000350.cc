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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Configure options using setters (available in the library)
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_perl_classes(opt, 0);
    cre2_opt_set_never_nl(opt, 1);

    // Use the required getters to validate option state
    int one_line = cre2_opt_one_line(opt);
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    int perl_classes = cre2_opt_perl_classes(opt);
    int never_nl = cre2_opt_never_nl(opt);

    if (one_line != 1) { cre2_opt_delete(opt); return 2; }
    if (enc != CRE2_UTF8) { cre2_opt_delete(opt); return 3; }
    if (perl_classes != 0) { cre2_opt_delete(opt); return 4; }
    if (never_nl != 1) { cre2_opt_delete(opt); return 5; }

    // Create a set using the options. Anchor value passed as 0.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) { cre2_opt_delete(opt); return 6; }

    // Add a couple of simple patterns and compile the set
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar(baz)");
    cre2_set_compile(set);

    // Prepare text and an integer array to receive match indices
    const char *text = "xx foobar(baz) yy";
    size_t text_len = 0;
    while (text[text_len]) ++text_len;

    int match_indices[8];
    for (size_t i = 0; i < sizeof(match_indices)/sizeof(match_indices[0]); ++i) match_indices[i] = -1;

    // Use the required cre2_set_match API
    size_t matches_found = cre2_set_match(set, text, text_len, match_indices, (int)(sizeof(match_indices)/sizeof(match_indices[0])));

    // Expect at least one match; otherwise treat as failure
    if (matches_found == 0) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 7;
    }

    // Clean up
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Success code as requested
    return 66;
}