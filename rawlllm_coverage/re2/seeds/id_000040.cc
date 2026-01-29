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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable word boundary behaviour
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_word_boundary(opt, 1);
    }

    // Create a set using the options (anchor value 0 cast to cre2_anchor_t)
    cre2_set *set = NULL;
    if (opt) {
        set = cre2_set_new(opt, (cre2_anchor_t)0);
        // Try adding a simple pattern and compile the set if creation succeeded
        if (set) {
            cre2_set_add_simple(set, "hello");
            cre2_set_compile(set);
        }
    }

    // Delete the set (use the required API)
    if (set) {
        cre2_set_delete(set);
        set = NULL;
    }

    // Prepare a pattern and compile a regexp with the same options
    const char *pattern = "(hello) world";
    int patlen = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, patlen, opt);

    // If compilation succeeded, query the number of capturing groups
    if (re) {
        int num_groups = cre2_num_capturing_groups(re);
        (void)num_groups; // use the value (silence unused variable warnings)
    }

    // Prepare input text and match storage for cre2_full_match
    const char *txt = "hello world";
    cre2_string_t text = { txt, (int)strlen(txt) };
    // Reserve space for whole match + one capturing group
    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    // Perform a full match using the pattern (non-compiled API)
    int fm = cre2_full_match(pattern, &text, matches, 2);
    (void)fm; // ignore result for this test sequence

    // Cleanup compiled regexp and options
    if (re) {
        cre2_delete(re);
        re = NULL;
    }
    if (opt) {
        cre2_opt_delete(opt);
        opt = NULL;
    }

    return 66;
}