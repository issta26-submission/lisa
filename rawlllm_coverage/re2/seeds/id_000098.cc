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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options */
    cre2_options_t *opt = cre2_opt_new();

    /* Compile a simple pattern with three capturing groups: (a)(b)(c) */
    const char *pattern = "(a)(b)(c)";
    int pattern_len = 9; /* length of "(a)(b)(c)" */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    if (re) {
        /* Retrieve an error argument container (will be empty for a successful compile,
           but we call the API as required) */
        cre2_string_t err_arg;
        err_arg.data = NULL;
        err_arg.length = 0;
        cre2_error_arg(re, &err_arg);

        /* Get the number of capturing groups and allocate match slots (group 0 + groups) */
        int num_groups = cre2_num_capturing_groups(re);
        int nmatch = num_groups + 1;
        cre2_string_t *matches = new cre2_string_t[nmatch];
        for (int i = 0; i < nmatch; ++i) {
            matches[i].data = NULL;
            matches[i].length = 0;
        }

        /* Perform a match against "abc" */
        const char *text = "abc";
        int textlen = 3;
        (void)cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, nmatch);

        /* Clean up */
        delete[] matches;
        cre2_delete(re);
    }

    if (opt) cre2_opt_delete(opt);

    return 66;
}