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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Query the current interface version (exercise the API)
    int ver = cre2_version_interface_current();
    (void)ver; // silence unused warning

    // Create options for compiling a regexp (optional but realistic)
    cre2_options_t *opt = cre2_opt_new();

    // Prepare an original pattern and compute its length without relying on external functions
    const char *pattern = "(a)(b)c";
    int pat_len = 0;
    while (pattern[pat_len] != '\0') ++pat_len;

    // Wrap the original pattern in a cre2_string_t
    cre2_string_t original;
    original.data = pattern;
    original.length = pat_len;

    // Prepare a cre2_string_t to receive the quoted (meta-escaped) version
    cre2_string_t quoted;
    quoted.data = NULL;
    quoted.length = 0;

    // Quote meta characters from the original pattern
    int quote_res = cre2_quote_meta(&quoted, &original);
    (void)quote_res; // we don't rely on the exact return here, just exercise the API

    // Choose which pattern buffer to pass to cre2_new:
    // prefer the quoted result if it was produced, otherwise use the original
    const char *use_pattern = original.data;
    int use_len = original.length;
    if (quoted.data != NULL && quoted.length > 0) {
        use_pattern = quoted.data;
        use_len = quoted.length;
    }

    // Compile the regexp using the chosen pattern
    cre2_regexp_t *re = cre2_new(use_pattern, use_len, opt);

    // Prepare a rewrite string to validate against the compiled regexp
    const char *rewrite_cstr = "$1-$2";
    int rewrite_len = 0;
    while (rewrite_cstr[rewrite_len] != '\0') ++rewrite_len;
    cre2_string_t rewrite;
    rewrite.data = rewrite_cstr;
    rewrite.length = rewrite_len;

    // Prepare an errmsg holder to receive any error message from check_rewrite_string
    cre2_string_t errmsg;
    errmsg.data = NULL;
    errmsg.length = 0;

    // If compilation succeeded, check the rewrite string against the regexp
    int check_res = 0;
    if (re != NULL) {
        check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    }
    (void)check_res; // not used further in this test harness
    (void)errmsg; // keep, even if empty

    // Clean up: delete the compiled regexp and the options
    if (re != NULL) {
        cre2_delete(re);
    }
    if (opt != NULL) {
        cre2_opt_delete(opt);
    }

    // Report success for the test harness
    return 66;
}