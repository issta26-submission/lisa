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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure encoding and perl classes as required.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_perl_classes(opt, 1);

    // Query version string (useful as part of a realistic API sequence).
    const char *ver = cre2_version_string();
    (void)ver; // silence unused-variable warnings

    // Simple utility to compute string length without relying on headers here.
    auto my_strlen = [](const char *s) -> int {
        if (!s) return 0;
        int l = 0;
        while (s[l]) ++l;
        return l;
    };

    // Build a pattern and create a regexp with the configured options.
    const char *pattern = "(foo)(?P<name>bar)";
    int pat_len = my_strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Prepare a rewrite string to be validated and an errmsg container for output.
    cre2_string_t rewrite;
    rewrite.data = "$1-$name";
    rewrite.length = my_strlen(rewrite.data);

    cre2_string_t errmsg;
    errmsg.data = NULL;
    errmsg.length = 0;

    // Check the rewrite string against the compiled regexp.
    int rc = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    (void)rc;    // accept any return for this test sequence
    (void)errmsg; // errmsg may be set by library; we don't need to free it here

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}