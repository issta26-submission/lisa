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
//<ID> 141
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

    // Configure options: set one-line mode and UTF-8 encoding.
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Verify the one_line flag was set (non-zero expected for true).
    if (!cre2_opt_one_line(opt)) {
        cre2_opt_delete(opt);
        return 2; // option not set as expected
    }

    // Create a new cre2 set using the prepared options, unanchored matching.
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 3; // failed to create set
    }

    // Add a couple of simple patterns to the set.
    int r1 = cre2_set_add_simple(set, "abc");
    int r2 = cre2_set_add_simple(set, "def.*");

    if (r1 < 0 || r2 < 0) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 4; // failed to add pattern(s)
    }

    // Clean up
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}