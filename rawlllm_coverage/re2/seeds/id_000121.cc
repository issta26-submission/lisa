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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query encoding
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failure to create options
    }
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; // silence unused warning

    // Create a regexp using the options, then delete it
    const char *pattern = "a+";
    cre2_regexp_t *re = cre2_new(pattern, 2, opt); // pattern length = 2 ("a+")
    if (re) {
        cre2_delete(re);
    }

    // Prepare input text and a match container, then call find_and_consume
    cre2_string_t text;
    text.data = "aaab";
    text.length = 4;

    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;

    int rc = cre2_find_and_consume(pattern, &text, &match, 1);
    (void)rc; // result is observed but not required for this sequence

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}