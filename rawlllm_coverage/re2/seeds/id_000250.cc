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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Enable one-line option and verify via getter */
    cre2_opt_set_one_line(opt, 1);
    int one_line_flag = cre2_opt_one_line(opt);
    if (!one_line_flag) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Create a set using the options (use a neutral anchor value) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 3;
    }

    /* Add a pattern to the set */
    const char *pattern = "\\b(hello)\\b";
    size_t pattern_len = (size_t)strlen(pattern);
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    int add_res = cre2_set_add(set, pattern, pattern_len, errbuf, sizeof(errbuf));
    if (add_res != 0) {
        /* If adding failed, clean up and return */
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 4;
    }

    /* Compile the set (best-effort; ignore nonzero/zero semantics) */
    (void)cre2_set_compile(set);

    /* Create a compiled regexp from the same pattern and options */
    cre2_regexp_t *re = cre2_new(pattern, (int)pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 5;
    }

    /* Create a named-groups iterator for the compiled regexp */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new((const cre2_regexp_t *)re);
    if (iter) {
        /* We don't need to iterate for this test; just delete the iterator */
        cre2_named_groups_iter_delete(iter);
    }

    /* Prepare strings for extract_re: text, rewrite and target placeholder */
    cre2_string_t text = { "hello world", (int)strlen("hello world") };
    cre2_string_t rewrite = { "$1", (int)strlen("$1") };
    cre2_string_t target = { NULL, 0 };

    /* Attempt to extract using the compiled regexp */
    int extract_res = cre2_extract_re(re, &text, &rewrite, &target);
    (void)extract_res; /* result is not critical for this sequence; ensure call succeeds syntactically */

    /* Clean up created objects */
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}