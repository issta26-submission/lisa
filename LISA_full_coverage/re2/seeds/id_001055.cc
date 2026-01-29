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
//<ID> 1055
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));
    int num_groups = 0;
    int prog_size = 0;
    int log_err_flag = 0;
    int64_t max_mem_val = 0;
    const char * pattern_from_re = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)4 * 1024 * 1024);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    max_mem_val = cre2_opt_max_mem(opt);
    num_groups = cre2_num_capturing_groups(re);
    prog_size = cre2_program_size(re);
    log_err_flag = cre2_opt_log_errors(opt);

    // step 4: Operate (compute lightweight checksum)
    computed_check = (long)pattern_len + (long)num_groups + (long)prog_size + (long)max_mem_val + (long)log_err_flag;
    (void)computed_check;
    (void)pattern_from_re;

    // step 5: Validate / compute (no branches per requirements)
    /* intentionally no conditional validation; checksum above captures state */

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}