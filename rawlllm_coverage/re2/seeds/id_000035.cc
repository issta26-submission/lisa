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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for set and regex creation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Create a set (unanchoered) and add patterns
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Add a couple of simple patterns. Use both a plain token and one with a named group.
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "(?P<name>\\w+)-(\\d+)");

    // Compile the set and check result
    int compile_ok = cre2_set_compile(set);
    if (!compile_ok) {
        // cleanup on failure
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 1;
    }

    // Create a regular expression with a named capture and a numeric capture
    const char *pattern = "(?P<name>\\w+)-(\\d+)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 1;
    }

    // Iterate named capturing groups using the iterator API
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *gname = NULL;
        int gindex = 0;
        while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
            // For fuzzing/exercise purposes we simply observe the name and index.
            // (No-op: keep variables used to avoid warnings.)
            (void)gname;
            (void)gindex;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Prepare to perform a match. Allocate match slots: 1 + number of capturing groups.
    int ngroups = cre2_num_capturing_groups(re);
    int nmatch = ngroups + 1;
    cre2_string_t *matches = new cre2_string_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Text to match against
    const char *text = "alice-42 and foo";
    int textlen = (int)strlen(text);

    // Perform a match (search unanchored over the whole text)
    int matched = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, nmatch);
    (void)matched; // we don't require a specific outcome for this test; keep result used to avoid warnings

    // Prepare a rewrite string and an error message holder, then validate the rewrite
    cre2_string_t rewrite;
    rewrite.data = "$1-$2"; // typical rewrite referencing capture groups
    rewrite.length = (int)strlen(rewrite.data);
    cre2_string_t errmsg;
    errmsg.data = NULL;
    errmsg.length = 0;

    // Check the rewrite string for validity against the compiled regexp
    int check_ok = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    (void)check_ok;
    (void)errmsg; // errmsg may be filled by the call; we simply accept it for this test

    // Cleanup allocated resources
    delete[] matches;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Return the required success code
    return 66;
}