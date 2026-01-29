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
//<ID> 1053
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([a-zA-Z]+)_(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * re_pat = NULL;
    int num_groups = 0;
    int64_t max_mem_before = 0;
    int64_t max_mem_after = 0;
    long computed_check = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)1048576);
    max_mem_before = cre2_opt_max_mem(opt);

    // step 3: Core operations (compile regexp and query pattern)
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);

    // step 4: Operate (query properties from compiled regexp and options)
    num_groups = cre2_num_capturing_groups(re);
    max_mem_after = cre2_opt_max_mem(opt);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)num_groups + (long)max_mem_before + (long)max_mem_after + (long)(re ? (long)re_pat : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}