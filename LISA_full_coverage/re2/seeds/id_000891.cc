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
//<ID> 891
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
    cre2_set * set = NULL;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    int one_line_flag = 0;
    int set_add_res = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    encoding = cre2_opt_encoding(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    set_add_res = cre2_set_add_simple(set, pattern);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)one_line_flag + (long)((int)encoding) + (long)set_add_res + (long)num_groups;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}