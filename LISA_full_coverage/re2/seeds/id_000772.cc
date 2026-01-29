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
//<ID> 772
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "([A-Z]+)([0-9]*)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    int num_groups = 0;
    int computed_check = 0;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Validate
    computed_check = pattern_len + (int)pattern_from_re[0] + num_groups;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}