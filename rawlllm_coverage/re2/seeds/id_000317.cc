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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Query version string
    const char *version = cre2_version_string();
    (void)version; // silence unused warning if any

    // Create options and query case sensitivity
    cre2_options_t *opt = cre2_opt_new();
    int case_sensitive = 0;
    if (opt) {
        case_sensitive = cre2_opt_case_sensitive(opt);
    }

    // Create a set with the options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Try adding a simple pattern to the set with an error buffer
    if (set) {
        const char *set_pattern = "(test)";
        char errbuf[128] = {0};
        (void)cre2_set_add(set, set_pattern, strlen(set_pattern), errbuf, sizeof(errbuf));
        // errbuf can be inspected in a real test; ignore here.
    }

    // Create a regexp with two capturing groups and query the count
    const char *regex_pattern = "(a)(b)";
    cre2_regexp_t *re = cre2_new(regex_pattern, (int)strlen(regex_pattern), opt);
    int n_captures = 0;
    if (re) {
        n_captures = cre2_num_capturing_groups(re);
        cre2_delete(re);
    }

    // Use values so compiler doesn't warn
    (void)case_sensitive;
    (void)n_captures;

    // Clean up
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}