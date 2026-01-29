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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set literal mode
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 67;
    cre2_opt_set_literal(opt, 1);

    // Create a set and add a simple literal pattern "abc"
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 68;
    }
    char errbuf[128] = {0};
    // pattern "abc" has length 3
    (void)cre2_set_add(set, "abc", (size_t)3, errbuf, sizeof(errbuf));
    // (ignore return value for fuzzing sequence; errbuf would contain message on error)

    // Create a regexp from the same literal pattern
    cre2_regexp_t *re = cre2_new("abc", 3, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 69;
    }

    // Prepare text and match storage for cre2_match
    const char *text = "abc123";
    int textlen = 6; // length of "abc123"
    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    // Attempt a full match (unanchored) over the text
    (void)cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, 2);

    // Demonstrate cre2_partial_match using pattern string form.
    cre2_string_t text_str;
    text_str.data = text;
    text_str.length = textlen;
    cre2_string_t groups[1];
    groups[0].data = NULL;
    groups[0].length = 0;
    // pattern "abc" as a C string; request 1 group (the whole match)
    (void)cre2_partial_match("abc", &text_str, groups, 1);

    // Clean up
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}