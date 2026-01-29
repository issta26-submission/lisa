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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);

    // Pattern and compiled regexp (using the same options)
    const char pattern[] = "(he)llo";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Text to consume from and array to receive capture groups
    const char text_literal[] = "hello world";
    cre2_string_t text = { text_literal, (int)(sizeof(text_literal) - 1) };

    // Prepare an array for matches (nmatch = 3: full match + one capture)
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Use the simple API variant that takes a pattern string
    int nmatch = 3;
    int rc = cre2_consume(pattern, &text, matches, nmatch);
    (void)rc; // result is not required for this test sequence

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}