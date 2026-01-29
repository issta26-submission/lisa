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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options for compiling regexes */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    /* Compile a regexp with named groups */
    const char *pattern = "(?P<key>[a-z]+)=(?P<val>\\d+)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Iterate named groups (if any) to exercise the iterator API */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = NULL;
        int index = 0;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            /* consume name/index; no-op but ensures iterator usage is exercised */
            (void)name;
            (void)index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    /* Use cre2_replace (pattern-based replace on a text buffer) */
    cre2_string_t text_and_target;
    text_and_target.data = "user=42 extra user=7";
    text_and_target.length = (int)strlen(text_and_target.data);

    cre2_string_t rewrite;
    rewrite.data = "UID:$2";
    rewrite.length = (int)strlen(rewrite.data);

    /* Call cre2_replace with the same pattern string (it is allowed) */
    int replace_result = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_result; /* ignore detailed result for this test */

    /* Use cre2_extract_re to extract using the compiled regexp */
    cre2_string_t text;
    text.data = "id=123 name=alice";
    text.length = (int)strlen(text.data);

    cre2_string_t extract_rewrite;
    extract_rewrite.data = "$key:$val";
    extract_rewrite.length = (int)strlen(extract_rewrite.data);

    cre2_string_t target;
    target.data = NULL;
    target.length = 0;

    int extract_result = cre2_extract_re(re, &text, &extract_rewrite, &target);
    (void)extract_result;
    (void)target; /* allow potential out params */

    /* Create and compile a set of patterns to exercise cre2_set_compile */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        /* add a simple pattern and compile the set */
        cre2_set_add_simple(set, "foo|bar");
        int set_compile_res = cre2_set_compile(set);
        (void)set_compile_res;
        cre2_set_delete(set);
    }

    /* Cleanup compiled regexp and options */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}