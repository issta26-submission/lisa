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
//<ID> 1373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)|bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xxfooyybarzz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * pattern_from_re = NULL;
    int literal_flag = 0;
    int ver_iface = 0;
    int num_groups = 0;
    int match_arr[4];
    int64_t max_mem_val = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)1024 * 1024); // set 1MB limit
    cre2_opt_set_literal(opt, 1);

    // step 3: Core operations
    max_mem_val = cre2_opt_max_mem(opt);
    literal_flag = cre2_opt_literal(opt);
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = (re != NULL) ? cre2_pattern(re) : NULL;
    num_groups = (re != NULL) ? cre2_num_capturing_groups(re) : 0;
    iter = cre2_named_groups_iter_new(re);
    ver_iface = cre2_version_interface_current();

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)literal_flag
                     + (long)ver_iface + (long)num_groups + (long)(re != NULL)
                     + (long)(iter != NULL) + (long)max_mem_val;
    (void)computed_check;
    (void)pattern_from_re;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}