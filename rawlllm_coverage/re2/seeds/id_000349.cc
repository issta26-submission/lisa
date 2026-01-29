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
//<ID> 349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set a couple of flags.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }
    // Use the required option-setting APIs.
    cre2_opt_set_never_capture(opt, 1);
    cre2_opt_set_posix_syntax(opt, 1);

    // Create a set with these options (anchor 0).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    // Prepare an error buffer for cre2_set_add.
    char errbuf[128] = {0};
    if (set) {
        // Add a simple pattern to the set.
        cre2_set_add(set, "abc", (size_t)3, errbuf, sizeof(errbuf));
    }

    // Create a compiled regexp using the same options.
    const char *pattern = "a(b)c";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), opt);

    // Prepare input for a full match call.
    cre2_string_t input_text;
    input_text.data = "abc";
    input_text.length = 3;

    // Prepare an array for match outputs (one whole match + one capture).
    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    if (rex) {
        // Attempt a full match using the compiled regexp.
        cre2_full_match_re(rex, &input_text, matches, 2);

        // Prepare text and rewrite strings for a global replace.
        cre2_string_t text_and_target;
        text_and_target.data = "abcabc";
        text_and_target.length = 6;

        cre2_string_t rewrite;
        rewrite.data = "X";
        rewrite.length = 1;

        // Perform a global replace using the compiled regexp.
        cre2_global_replace_re(rex, &text_and_target, &rewrite);
    }

    // Clean up resources.
    if (rex) cre2_delete(rex);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}