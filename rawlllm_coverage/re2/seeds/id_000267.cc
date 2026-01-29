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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options (use cre2_opt_new which is part of the available API)
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, nothing else to do
        return 66;
    }

    // Set case sensitivity on the options
    cre2_opt_set_case_sensitive(opt, 1);

    // Prepare an original string to be quoted as a regex meta-safe pattern
    cre2_string_t original;
    original.data = "a+b";
    original.length = 3;

    // Prepare a cre2_string_t to receive the quoted result
    cre2_string_t quoted;
    quoted.data = NULL;
    quoted.length = 0;

    // Quote the original string (escape regex metacharacters)
    // cre2_quote_meta will populate 'quoted' (implementation-defined allocation/ownership)
    (void)cre2_quote_meta(&quoted, &original);

    // Use the quoted result as the pattern for creating a regexp.
    // If quoting failed or did not produce data, fall back to the original.
    const char *pattern = quoted.data ? quoted.data : original.data;
    int pattern_len = quoted.length ? quoted.length : original.length;

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        // Clean up the compiled regexp
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}