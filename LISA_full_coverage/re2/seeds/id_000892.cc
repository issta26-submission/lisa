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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern1[] = "^(foo)(bar)$";
    const int pattern1_len = (int)(sizeof(pattern1) - 1);
    const char pattern2[] = "foo";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int one_line_set_flag = 1;
    int one_line_read_flag = 0;
    int add_simple_result = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, one_line_set_flag);
    enc = cre2_opt_encoding(opt);
    one_line_read_flag = cre2_opt_one_line(opt);

    // step 3: Core operations
    re = cre2_new(pattern1, pattern1_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_simple_result = cre2_set_add_simple(set, pattern2);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)pattern1_len + (long)enc + (long)one_line_read_flag + (long)add_simple_result + (long)num_groups;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}