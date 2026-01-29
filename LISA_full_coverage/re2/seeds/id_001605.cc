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
//<ID> 1605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)baz$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int ver_rev = 0;
    int log_flag_set = 1;
    int one_line_flag_set = 1;
    int log_flag_get = 0;
    int one_line_flag_get = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, log_flag_set);
    cre2_opt_set_one_line(opt, one_line_flag_set);
    log_flag_get = cre2_opt_log_errors(opt);
    one_line_flag_get = cre2_opt_one_line(opt);

    // step 3: Configure
    ver_rev = cre2_version_interface_revision();
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    checksum = 0;
    checksum += (long)pattern_len;
    checksum += (long)ver_rev;
    checksum += (long)log_flag_get;
    checksum += (long)one_line_flag_get;
    checksum += (long)(re != NULL);

    // step 5: Validate
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}