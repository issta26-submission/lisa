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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_word_boundary(opt, 1);
    }

    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        /* add a simple pattern to the set and attempt to compile it */
        cre2_set_add_simple(set, "foo");
        (void)cre2_set_compile(set);
    }

    /* Prepare a match slot and run an easy match (pattern vs text) */
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;
    (void)cre2_easy_match("foo", 3, "afoot", 5, &match, 1);

    /* Create a regexp object and immediately delete it to exercise cre2_delete */
    cre2_regexp_t *re = cre2_new("bar", 3, opt);
    if (re) {
        cre2_delete(re);
    }

    if (set) {
        cre2_set_delete(set);
    }
    if (opt) {
        cre2_opt_delete(opt);
    }

    return 66;
}