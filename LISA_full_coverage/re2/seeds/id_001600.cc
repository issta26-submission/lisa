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
//<ID> 1600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int ver_rev = 0;
    int set_log = 1;
    int set_one_line = 1;
    int got_log = 0;
    int got_one_line = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, set_log);
    cre2_opt_set_one_line(opt, set_one_line);

    // step 3: Configure
    got_log = cre2_opt_log_errors(opt);
    got_one_line = cre2_opt_one_line(opt);

    // step 4: Operate
    ver_rev = cre2_version_interface_revision();
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate
    checksum = (long)pattern_len + (long)ver_rev + (long)set_log + (long)set_one_line + (long)got_log + (long)got_one_line + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}