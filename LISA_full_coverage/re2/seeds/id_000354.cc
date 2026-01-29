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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char original_buf[] = "a+b?^d$[x]";
    const int original_len = (int)(sizeof(original_buf) - 1);
    char quoted_buf[128];
    cre2_string_t original;
    cre2_string_t quoted;
    cre2_options_t *opt = NULL;
    int rc_quote = 0;
    int ver_current = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(quoted_buf, 0, sizeof(quoted_buf));
    original.data = original_buf;
    original.length = original_len;
    quoted.data = quoted_buf;
    quoted.length = (int)(sizeof(quoted_buf) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Operate
    rc_quote = cre2_quote_meta(&quoted, &original);
    ver_current = cre2_version_interface_current();

    // step 5: Validate
    computed_check = rc_quote + ver_current + quoted.length + original.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}