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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set a couple of flags.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // Prepare input text and match storage.
    const char *pattern = "\\bhello\\b"; // look for whole word "hello"
    cre2_string_t text = { "hello world", 11 };
    cre2_string_t matches[4];
    for (int i = 0; i < 4; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Attempt a partial match (pattern against the single input string).
    int nmatch = 1; // request only the overall match
    int rc = cre2_partial_match(pattern, &text, matches, nmatch);

    // Silence unused variable warning; we could inspect rc or matches if desired.
    (void)rc;

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}