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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }

    // Configure options: set encoding, allow '.' to match newline, and limit memory
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(4LL * 1024LL * 1024LL)); // 4 MB

    const char *pattern = "^(hello).*world$";
    int pattern_len = (int)strlen(pattern);

    // Create regexp using the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Options can be deleted after creating the regexp (regexp keeps needed state)
    cre2_opt_delete(opt);

    if (!re) {
        return 2; // failed to compile regexp
    }

    // Check for any error string associated with the compiled regexp
    const char *err = cre2_error_string(re);
    if (err && err[0] != '\0') {
        cre2_delete(re);
        return 3; // compilation produced an error message
    }

    // Clean up regexp
    cre2_delete(re);

    return 66;
}