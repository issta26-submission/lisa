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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set logging
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_log_errors(opt, 1);
        // Query the never-capture flag (default); store to avoid unused-result warnings
        int never_capture_flag = cre2_opt_never_capture(opt);
        (void)never_capture_flag;
    }

    // Prepare text and rewrite buffers (stack-allocated to avoid heap management)
    char textbuf[] = "hello world";
    cre2_string_t text_and_target = { textbuf, (int)(sizeof(textbuf) - 1) };

    char rewritebuf[] = "earth";
    cre2_string_t rewrite = { rewritebuf, (int)(sizeof(rewritebuf) - 1) };

    // Perform a simple replacement: "world" -> "earth"
    int replace_result = cre2_replace("world", &text_and_target, &rewrite);
    (void)replace_result; // ignore return for this test sequence

    // Clean up options
    if (opt) cre2_opt_delete(opt);

    return 66;
}