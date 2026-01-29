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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and inspect/modify some flags.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, nothing else can be done.
        return 66;
    }

    // Query whether "perl classes" mode is currently enabled.
    int perl_classes_enabled = cre2_opt_perl_classes(opt);
    (void)perl_classes_enabled; // silence unused-variable warnings

    // Set the "never capture" option to true to exercise that API.
    cre2_opt_set_never_capture(opt, 1);

    // Build a simple pattern that would normally capture digits.
    const char *pattern = "([0-9]+)";
    int pattern_len = (int)strlen(pattern);

    // Compile the regexp with the options we've configured.
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare the input text and the rewrite/template for extraction.
    const char *input = "prefix12345suffix";
    cre2_string_t text = { input, (int)strlen(input) };
    cre2_string_t rewrite = { "$1", (int)strlen("$1") };

    // Prepare a buffer for the extraction target. cre2_extract_re may populate
    // the target's data/length fields; provide a buffer in case it's used.
    char target_buf[128] = {0};
    cre2_string_t target = { target_buf, 0 };

    // If compilation succeeded, attempt to extract using the compiled regex.
    if (re) {
        // Run the extract API that uses a compiled regexp.
        // We ignore the specific return value here; the call exercises the API.
        (void)cre2_extract_re(re, &text, &rewrite, &target);

        // Clean up the compiled regexp.
        cre2_delete(re);
    }

    // Clean up options.
    cre2_opt_delete(opt);

    // Success return as requested.
    return 66;
}