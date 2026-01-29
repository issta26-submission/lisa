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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;

    /* configure options */
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    /* create a set with the options; use anchor value 0 */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return -2;
    }

    /* add a few simple patterns */
    const char *patterns[] = {
        "hello",
        "world.*",
        "^start",
        "end$"
    };
    for (size_t i = 0; i < sizeof(patterns) / sizeof(patterns[0]); ++i) {
        /* ignore individual add return codes for this sequence */
        (void)cre2_set_add_simple(set, patterns[i]);
    }

    /* compile the set (ignore the specific return value here) */
    (void)cre2_set_compile(set);

    /* cleanup */
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}