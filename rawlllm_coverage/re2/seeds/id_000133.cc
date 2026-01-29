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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // Enable word-boundary option
    cre2_opt_set_word_boundary(opt, 1);

    // Query max memory setting (just to exercise the getter)
    int64_t max_mem = cre2_opt_max_mem(opt);
    (void)max_mem; // silence unused variable warning

    // Create a set with the configured options and an anchor
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);
    if (set) {
        // Add a simple pattern to the set
        cre2_set_add_simple(set, "hello");
    }

    // Prepare an input string for partial match
    const char sample_text[] = "well hello there";
    cre2_string_t text;
    text.data = sample_text;
    text.length = (int)strlen(sample_text);

    // Prepare match output array (two slots: whole match + one capture as an example)
    cre2_string_t matches[2];
    matches[0].data = nullptr;
    matches[0].length = 0;
    matches[1].data = nullptr;
    matches[1].length = 0;

    // Use cre2_partial_match to attempt a partial match of the pattern "hello"
    int pm_result = cre2_partial_match("hello", &text, matches, 2);
    (void)pm_result; // ignore result for this test harness

    // Cleanup
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}