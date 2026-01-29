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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still return success code per requirements
    }

    // Set some option flags (use setters available in the library)
    cre2_opt_set_literal(opt, 0);   // turn off literal mode
    cre2_opt_set_dot_nl(opt, 1);    // allow '.' to match newlines

    // Use the required getters to observe the option state
    int literal_flag = cre2_opt_literal(opt);
    int dotnl_flag = cre2_opt_dot_nl(opt);
    (void)literal_flag; // silence unused-variable warnings
    (void)dotnl_flag;

    // Compile a simple regular expression using these options
    const char *pattern = "(foo)(bar)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);

    // If compilation succeeded, exercise named-group iterator and global replace
    cre2_named_groups_iter_t *iter = nullptr;
    if (rex) {
        // create named-groups iterator (may be null if no named groups)
        iter = cre2_named_groups_iter_new(rex);
        if (iter) {
            const char *name = nullptr;
            int index = 0;
            // attempt one iteration (safe even if there are no named groups)
            (void)cre2_named_groups_iter_next(iter, &name, &index);
            // delete the iterator using the required API
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }

        // Prepare strings for global replace using the compiled regexp
        cre2_string_t text_and_target;
        cre2_string_t rewrite;
        const char *text_literal = "foobar and foobar";
        text_and_target.data = text_literal;
        text_and_target.length = (int)strlen(text_literal);

        const char *rewrite_literal = "X";
        rewrite.data = rewrite_literal;
        rewrite.length = (int)strlen(rewrite_literal);

        // Perform a global replace using the compiled regexp
        // (we intentionally ignore the return value except to prevent warnings)
        int replace_rc = cre2_global_replace_re(rex, &text_and_target, &rewrite);
        (void)replace_rc;

        // Clean up the compiled regexp
        cre2_delete(rex);
        rex = nullptr;
    }

    // Demonstrate usage of cre2_set and cre2_set_match
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // add a simple pattern, compile the set
        cre2_set_add_simple(set, "foo");
        cre2_set_compile(set);

        // prepare match buffer and run match
        int match_buf[8] = {0};
        size_t max_matches = sizeof(match_buf) / sizeof(match_buf[0]);
        const char *subject = "foo bar baz";
        size_t subject_len = strlen(subject);
        size_t found = cre2_set_match(set, subject, subject_len, match_buf, max_matches);
        (void)found; // suppress unused warning

        // delete the set
        cre2_set_delete(set);
        set = nullptr;
    }

    // Clean up options
    cre2_opt_delete(opt);
    opt = nullptr;

    return 66;
}