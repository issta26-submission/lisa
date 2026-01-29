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
//<ID> 359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    char original_buf[] = "a+b?c^d$[](){}|\\";
    char quoted_buf[128];
    cre2_options_t * opt = NULL;
    cre2_string_t original;
    cre2_string_t quoted;
    int quote_result = 0;
    int version_current = 0;
    int case_flag = 1;
    int computed_check = 0;

    // step 2: Setup
    memset(quoted_buf, 0, sizeof(quoted_buf));
    original.data = original_buf;
    original.length = (int)(sizeof(original_buf) - 1);
    quoted.data = quoted_buf;
    quoted.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, case_flag);

    // step 4: Operate
    quote_result = cre2_quote_meta(&quoted, &original);

    // step 5: Validate
    version_current = cre2_version_interface_current();
    computed_check = quote_result + version_current + quoted.length + original.length + case_flag;
    (void)computed_check;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}