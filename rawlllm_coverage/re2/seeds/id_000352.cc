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
//<ID> 352
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

    // Set maximum memory allowed for the compiled regexp
    cre2_opt_set_max_mem(opt, (int64_t)1024LL * 1024LL); // 1 MiB

    // Use setters to configure options (setters are available in the API list)
    cre2_opt_set_literal(opt, 1);   // treat pattern as literal by default
    cre2_opt_set_dot_nl(opt, 0);    // '.' does not match newline

    // Use the required getters to inspect options
    int literal_enabled = cre2_opt_literal(opt);
    int dot_nl_enabled = cre2_opt_dot_nl(opt);
    (void)literal_enabled; // silence unused-variable warnings
    (void)dot_nl_enabled;

    // Prepare a pattern and compile it with the options
    const char *pattern = "(foo).*bar";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare a cre2_string_t to receive any error argument information
    cre2_string_t err_arg;
    err_arg.data = nullptr;
    err_arg.length = 0;

    // Call the required function to obtain the error argument (if any)
    cre2_error_arg(re, &err_arg);

    // Clean up resources
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}