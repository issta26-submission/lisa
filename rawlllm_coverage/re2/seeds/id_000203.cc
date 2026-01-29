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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* enable perl classes behavior on options */
    cre2_opt_set_perl_classes(opt, 1);

    /* create a regexp with a named capturing group */
    const char *pattern = "(?P<name>ab)c";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);

    /* iterate named groups if regexp was created */
    cre2_named_groups_iter_t *iter = NULL;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }
    if (iter) {
        const char *gname = NULL;
        int gindex = 0;
        /* loop through available named groups (if any) */
        while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
            /* use name and index in a no-op way to avoid unused warnings */
            (void)gname;
            (void)gindex;
        }
        /* required cleanup of the iterator */
        cre2_named_groups_iter_delete(iter);
        iter = NULL;
    }

    /* create a set, add patterns, compile it, then delete it */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "foo");
        cre2_set_add_simple(set, "bar");
        /* call the required compile API */
        int compile_res = cre2_set_compile(set);
        (void)compile_res;
        cre2_set_delete(set);
        set = NULL;
    }

    /* prepare a cre2_string_t for full match testing */
    cre2_string_t txt;
    txt.data = "abc";
    txt.length = 3;

    /* call cre2_full_match with a simple pattern; allow zero capture slots */
    int fm = cre2_full_match("a(b)c", &txt, NULL, 0);
    (void)fm;

    /* cleanup regexp and options */
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}