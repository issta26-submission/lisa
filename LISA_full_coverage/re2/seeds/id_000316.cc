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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>abc)def";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char work_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * errstr = NULL;
    int case_flag = 1;
    int one_line_flag = 1;
    int first_char = 0;

    // step 2: Setup
    memset(work_buf, 0, sizeof(work_buf));
    memcpy(work_buf, "initial", 7);

    // step 3: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    cre2_opt_set_one_line(opt, one_line_flag);

    // step 4: Operate / Validate
    re = cre2_new(pattern, pattern_len, opt);
    errstr = cre2_error_string(re);
    first_char = errstr ? (int)(unsigned char)errstr[0] : 0;
    work_buf[0] = (char)((unsigned char)work_buf[0] + (unsigned)((first_char & 0x7) + (case_flag & 0x1) + (one_line_flag & 0x1)));
    iter = cre2_named_groups_iter_new(re);

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}