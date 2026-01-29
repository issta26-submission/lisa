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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for compilation and a set container.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Create a pattern set (anchor unanchored).
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set and compile it.
    // Ignore return values for the purposes of this sequence.
    cre2_set_add_simple(set, "simple");
    cre2_set_compile(set);

    // Query the library interface age (demonstration of usage).
    int age = cre2_version_interface_age();
    (void)age; // silence unused-warning; value may be used in real checks.

    // Compile a regular expression with a named capturing group "g".
    const char *pattern = "(?P<g>ab)";
    cre2_regexp_t *re = cre2_new(pattern, 9, opt); // pattern length = 9
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Find the index of the named capturing group "g".
    int gindex = cre2_find_named_capturing_groups(re, "g");
    (void)gindex; // in a test harness we might assert on this value

    // Prepare text and replacement for replace operation.
    cre2_string_t text_and_target;
    text_and_target.data = "xxabyy";
    text_and_target.length = 6;

    cre2_string_t rewrite;
    rewrite.data = "X";
    rewrite.length = 1;

    // Perform a single replace using the compiled regexp.
    int replace_result = cre2_replace_re(re, &text_and_target, &rewrite);
    (void)replace_result; // ignore specific return for this sequence

    // Demonstrate cre2_find_and_consume with a simple pattern string.
    cre2_string_t input;
    input.data = "ab##";
    input.length = 4;

    // Prepare an array for matches (two slots).
    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    // Call find_and_consume using a simple literal pattern.
    int fac_result = cre2_find_and_consume("ab", &input, matches, 2);
    (void)fac_result;

    // Clean up allocated objects.
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Indicate success as required.
    return 66;
}