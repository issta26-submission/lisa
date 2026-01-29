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
//<ID> 332
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
    cre2_opt_set_posix_syntax(opt, 1);

    // Check interface revision (usage demonstration)
    int rev = cre2_version_interface_revision();
    (void)rev; // suppress unused-variable warnings in some builds

    // Prepare a pattern and compile it
    const char *pattern = "(ab)c";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare text and match slots
    const char *text = "xxabcyy";
    int text_len = (int)strlen(text);
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) { matches[i].data = NULL; matches[i].length = 0; }

    // Attempt a match: look for the pattern anywhere in the text
    int matched = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
    if (matched <= 0) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Demonstrate cre2_quote_meta: quote the original pattern
    cre2_string_t orig;
    orig.data = pattern;
    orig.length = pattern_len;
    cre2_string_t quoted;
    quoted.data = NULL;
    quoted.length = 0;
    (void)cre2_quote_meta(&quoted, &orig); // ignore result but exercise the API

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}