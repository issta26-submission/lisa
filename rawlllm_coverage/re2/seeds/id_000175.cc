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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Create a pattern (use array so we can compute length without strlen) */
    const char pattern[] = "a(b)c";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare an input string to search/consume from (array so length is known) */
    char input_buf[] = "abc abc";
    cre2_string_t input = { input_buf, (int)(sizeof(input_buf) - 1) };
    cre2_string_t match = { NULL, 0 };

    /* Use cre2_find_and_consume with the pattern string (this will try to find and consume) */
    /* nmatch set to 1 to request a single match/capture slot */
    int res_find = cre2_find_and_consume(pattern, &input, &match, 1);
    (void)res_find; /* ignore result for this test; just exercise the API */

    /* Create a set, add a simple pattern, then delete the set */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        int add_res = cre2_set_add_simple(set, "abc");
        (void)add_res;
        cre2_set_delete(set); /* required API usage */
    }

    /* Clean up regexp and options */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}