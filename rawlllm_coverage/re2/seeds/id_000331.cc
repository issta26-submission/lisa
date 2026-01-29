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
//<ID> 331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;

    // Configure options (use setters available in the API list)
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // Read back using the required getter APIs
    int word_boundary = cre2_opt_word_boundary(opt);
    int case_sensitive = cre2_opt_case_sensitive(opt);

    // Prepare a pattern and compile with the options
    const char pattern[] = "\\btest\\b";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return -2;
    }

    // Clean up compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Use the read-back values to avoid unused-variable warnings
    if (!(word_boundary == 1 && case_sensitive == 1)) {
        // If values aren't as expected, still consider this a failure.
        return -3;
    }

    return 66;
}