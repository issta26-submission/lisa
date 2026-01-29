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
//<ID> 350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    cre2_options_t * opt = NULL;
    cre2_string_t original;
    cre2_string_t quoted;
    char original_buf[] = "a+b?^$.*()[]{}|\\hello";
    char quoted_buf[256];
    int version_cur = 0;
    int quote_rc = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(quoted_buf, 0, sizeof(quoted_buf));
    original.data = original_buf;
    original.length = (int)(sizeof(original_buf) - 1);
    quoted.data = quoted_buf;
    quoted.length = (int)sizeof(quoted_buf);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    version_cur = cre2_version_interface_current();

    // step 4: Operate
    quote_rc = cre2_quote_meta(&quoted, &original);

    // step 5: Validate
    computed_check = quote_rc + version_cur + original.length + (int)(unsigned char)quoted.data[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);
    memset(quoted_buf, 0, sizeof(quoted_buf));

    // API sequence test completed successfully
    return 66;
}