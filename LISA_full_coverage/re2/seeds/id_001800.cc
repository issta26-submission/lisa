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
//<ID> 1800
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char work_buf[64];
    memset(work_buf, 0, sizeof(work_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * ng_name = NULL;
    int ng_index = -1;
    int num_caps = 0;
    int rev = 0;
    bool iter_next_res = false;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate
    num_caps = cre2_num_capturing_groups(re);
    iter = cre2_named_groups_iter_new(re);
    iter_next_res = cre2_named_groups_iter_next(iter, &ng_name, &ng_index);
    rev = cre2_version_interface_revision();

    // step 4: Validate
    checksum = (long)pattern_len + (long)num_caps + (long)rev + (long)(iter_next_res ? 1 : 0) + (long)ng_index + (long)(ng_name != NULL) + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}