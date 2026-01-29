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
//<ID> 263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Set a reasonable memory limit for compilation/execution */
    cre2_opt_set_max_mem(opt, (int64_t)(1024LL * 1024LL)); /* 1 MiB */

    /* Query the 'literal' option (use the getter at least once) */
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; /* silence unused-variable warnings */

    /* Prepare a pattern with three capturing groups: (a) (b(c)) => 3 groups */
    const char pattern[] = "(a)(b(c))";
    int patlen = (int)(sizeof(pattern) - 1);

    cre2_regexp_t *re = cre2_new(pattern, patlen, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    int groups = cre2_num_capturing_groups(re);
    /* Verify we got the expected number of capturing groups */
    if (groups != 3) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    cre2_delete(re);
    cre2_opt_delete(opt);
    return 66;
}