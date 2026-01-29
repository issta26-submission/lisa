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
//<ID> 1606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int ver_rev = 0;
    int num_groups = 0;
    int log_flag = 0;
    int one_line_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    ver_rev = cre2_version_interface_revision();

    // step 4: Operate
    num_groups = cre2_num_capturing_groups(re);
    log_flag = cre2_opt_log_errors(opt);
    one_line_flag = cre2_opt_one_line(opt);

    // step 5: Validate
    checksum = (long)pattern_len + (long)ver_rev + (long)num_groups + (long)log_flag + (long)one_line_flag + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}