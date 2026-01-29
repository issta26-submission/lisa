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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Query version interface (use result to avoid unused-value warnings)
    int ver = cre2_version_interface_current();
    (void)ver;

    // Create options and set encoding
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Create a set with the options. Use a zero anchor (cast to cre2_anchor_t).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a couple of patterns to the set, capturing any error text.
    const char *p1 = "hello";
    const char *p2 = "^world$";
    char errbuf[128] = {0};

    size_t len1 = strlen(p1);
    int r1 = cre2_set_add(set, p1, len1, errbuf, sizeof(errbuf));
    // Ignore specific return semantics here, but keep error text if any.
    (void)r1;

    // Clear error buffer and add second pattern
    errbuf[0] = '\0';
    size_t len2 = strlen(p2);
    int r2 = cre2_set_add(set, p2, len2, errbuf, sizeof(errbuf));
    (void)r2;

    // Cleanup set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}