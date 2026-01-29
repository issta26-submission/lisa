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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    cre2_options_t * opt = NULL;
    const char original_buf[] = "a+b(c)*?";
    cre2_string_t original;
    char quoted_buf[64];
    cre2_string_t quoted;
    int quote_result = 0;
    int version_cur = 0;
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

    // step 4: Operate
    quote_result = cre2_quote_meta(&quoted, &original);

    // step 5: Validate
    version_cur = cre2_version_interface_current();
    computed_check = quote_result + version_cur + quoted.length + original.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}