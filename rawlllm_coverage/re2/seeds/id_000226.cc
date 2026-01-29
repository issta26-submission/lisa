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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Query several option flags to exercise the getters.
    int perl_classes = cre2_opt_perl_classes(opt);
    int longest_match = cre2_opt_longest_match(opt);
    int one_line = cre2_opt_one_line(opt);

    // Use the retrieved values in a trivial way to avoid unused-variable warnings.
    volatile int sanity = perl_classes ^ longest_match ^ one_line;
    (void)sanity;

    // Call cre2_delete with a null regexp pointer to exercise the deletion path safely.
    cre2_regexp_t *re = nullptr;
    cre2_delete(re);

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}