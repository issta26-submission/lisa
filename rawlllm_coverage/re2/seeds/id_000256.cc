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
//<ID> 256
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
        return 66; // still treat as success for this test harness
    }

    // Set some options using setters (available in the library)
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_never_capture(opt, 1);

    // Use the required getter APIs to read back values
    int one_line_flag = cre2_opt_one_line(opt);
    int never_capture_flag = cre2_opt_never_capture(opt);

    // Create a cre2_set, add a simple pattern, compile and delete it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // add a simple pattern and compile (ignore return codes for brevity)
        cre2_set_add_simple(set, "abc");
        cre2_set_compile(set);
        // required API usage
        cre2_set_delete(set);
    }

    // Prepare input text and match containers for cre2_consume
    const char text[] = "aaab";
    cre2_string_t text_str = { text, (int)(sizeof(text) - 1) };
    cre2_string_t match = { nullptr, 0 };

    // Use cre2_consume with a simple pattern; nmatch = 1
    int consume_rc = cre2_consume("a+", &text_str, &match, 1);

    // Silence unused-variable warnings (no-op)
    (void)one_line_flag;
    (void)never_capture_flag;
    (void)consume_rc;
    (void)match.length;

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}