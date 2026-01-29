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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(abc)(def)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char workbuf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * errstr = NULL;
    int one_line_flag = 1;
    int case_flag = 1;

    // step 2: Setup
    memset(workbuf, 0, sizeof(workbuf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, one_line_flag);
    cre2_opt_set_case_sensitive(opt, case_flag);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate
    errstr = cre2_error_string(re);
    workbuf[0] = errstr ? (char)errstr[0] : 0;
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_delete(iter);

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}