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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set a reasonable max memory limit (8 MiB).
    cre2_opt_set_max_mem(opt, (int64_t)8 * 1024 * 1024);

    // Query several option getters required by the task.
    int never_capture = cre2_opt_never_capture(opt);
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    int dot_nl = cre2_opt_dot_nl(opt);

    // Basic validation of returned values to ensure they are in expected ranges.
    if (!(never_capture == 0 || never_capture == 1)) {
        cre2_opt_delete(opt);
        return 2;
    }
    if (!(enc == CRE2_UNKNOWN || enc == CRE2_UTF8 || enc == CRE2_Latin1)) {
        cre2_opt_delete(opt);
        return 3;
    }
    if (!(dot_nl == 0 || dot_nl == 1)) {
        cre2_opt_delete(opt);
        return 4;
    }

    // Change max mem again to exercise the setter.
    cre2_opt_set_max_mem(opt, (int64_t)0);

    cre2_opt_delete(opt);
    return 66;
}