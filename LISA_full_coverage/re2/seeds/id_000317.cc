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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>abc)(def)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char work_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * errstr = NULL;
    const char * gname = NULL;
    int gindex = -1;

    // step 2: Setup
    memset(work_buf, 0, sizeof(work_buf));
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);

    // step 3: Core operations (create regexp and obtain error/info)
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);

    // step 4: Operate / Validate (iterate named groups once and mix values into buffer)
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &gname, &gindex);
    work_buf[0] = (char)((unsigned char)work_buf[0] + (unsigned char)((gindex & 0xFF) + (int)errstr[0]));

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}