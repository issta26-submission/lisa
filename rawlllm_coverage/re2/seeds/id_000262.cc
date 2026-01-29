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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable word-boundary handling in the options
    cre2_opt_set_word_boundary(opt, 1);

    // Query the encoding from the options (exercise the API)
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; // suppress unused variable warnings

    // Prepare a pattern that uses word boundary escapes
    const char pattern[] = "\\ba\\b";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regular expression with the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Clean up compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}