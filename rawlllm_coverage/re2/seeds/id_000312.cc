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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create default options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Configure an option to exercise option setters (optional)
    cre2_opt_set_case_sensitive(opt, 1);

    // Pattern with a named group "foo" and a second unnamed capturing group
    const char pattern[] = "(?P<foo>abc)(def)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile a regexp with the options
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query an "error argument" structure slot (demonstrates cre2_error_arg usage)
    cre2_string_t arg = { NULL, 0 };
    cre2_error_arg(rex, &arg);
    // arg may now point into regexp internals (we don't dereference here)

    // Create a named-groups iterator and try to fetch one group name/index
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(rex);
    if (iter) {
        const char *gname = NULL;
        int gindex = -1;
        // Attempt to get the next named group (if any). We intentionally ignore result details.
        bool has_next = cre2_named_groups_iter_next(iter, &gname, &gindex);
        (void)has_next;
        (void)gname;
        (void)gindex;
        cre2_named_groups_iter_delete(iter);
    }

    // Create a set using the same options (demonstrates cre2_set_new usage)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set) {
        // Add a simple pattern and compile the set (use safe simple API)
        cre2_set_add_simple(set, "abc");
        cre2_set_compile(set);
        cre2_set_delete(set);
    }

    // Prepare a rewrite string and an errmsg container to check the rewrite string
    const char rewrite_data[] = "\\1"; // backreference style
    cre2_string_t rewrite = { rewrite_data, (int)(sizeof(rewrite_data) - 1) };
    cre2_string_t errmsg = { NULL, 0 };

    // Check the rewrite string against the compiled regexp
    // We do not enforce success here; this call shows usage of cre2_check_rewrite_string.
    int check_result = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
    (void)check_result;
    (void)errmsg;

    // Clean up regexp and options
    cre2_delete(rex);
    cre2_opt_delete(opt);

    return 66;
}