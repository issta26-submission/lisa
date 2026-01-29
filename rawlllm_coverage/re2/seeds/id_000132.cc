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
//<ID> 132
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set flags: disable capturing and ensure pattern is treated as a regex (not literal)
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_never_capture(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // Build a simple pattern and compile with the options
    const char pattern[] = "(foo)(bar)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Prepare a valid rewrite string (no backreferences)
    const char good_rewrite_data[] = "replacement";
    cre2_string_t good_rewrite;
    good_rewrite.data = good_rewrite_data;
    good_rewrite.length = (int)(sizeof(good_rewrite_data) - 1);

    // Prepare an errmsg container (output)
    cre2_string_t errmsg;
    errmsg.data = nullptr;
    errmsg.length = 0;

    // Check the valid rewrite string; expect success (non-zero)
    int ok = cre2_check_rewrite_string(re, &good_rewrite, &errmsg);
    if (!ok) {
        // Unexpected: cleanup and fail
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 1;
    }

    // Prepare an invalid rewrite that uses a backreference ("\1").
    // Since never_capture was set, this should be invalid; check it as well.
    const char bad_rewrite_data[] = "\\1";
    cre2_string_t bad_rewrite;
    bad_rewrite.data = bad_rewrite_data;
    bad_rewrite.length = (int)(sizeof(bad_rewrite_data) - 1);

    // Reset errmsg
    errmsg.data = nullptr;
    errmsg.length = 0;

    // It's acceptable whether this returns success or failure depending on implementation details;
    // we call it to exercise the API. Do not treat its result as fatal.
    (void)cre2_check_rewrite_string(re, &bad_rewrite, &errmsg);

    // Clean up and report success
    cre2_delete(re);
    cre2_opt_delete(opt);
    return 66;
}