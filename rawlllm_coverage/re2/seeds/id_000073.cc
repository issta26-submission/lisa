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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create default options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Query library interface version (keep the value to show we used the call)
    int ver = cre2_version_interface_current();

    // Prepare a simple pattern and compile it using the options
    const char *pattern = "(test)(ing)";
    int pattern_len = (int)(sizeof("(test)(ing)") - 1);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);

    // Options can be deleted after creating the regexp
    cre2_opt_delete(opt);

    // Ensure regex was created
    if (!rex) {
        return 1;
    }

    // Prepare a rewrite string to validate against the compiled regexp
    cre2_string_t rewrite;
    rewrite.data = "replacement";
    rewrite.length = (int)(sizeof("replacement") - 1);

    // Prepare an error message buffer (if the check reports an error)
    char errmsg_buf[128] = {0};
    cre2_string_t errmsg;
    errmsg.data = errmsg_buf;
    errmsg.length = (int)sizeof(errmsg_buf);

    // Check the rewrite string for validity relative to the compiled regexp
    int check_rc = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
    (void)check_rc; // result intentionally unused for this synthetic test
    (void)ver;      // version intentionally unused further

    // Successful path for this test harness
    return 66;
}