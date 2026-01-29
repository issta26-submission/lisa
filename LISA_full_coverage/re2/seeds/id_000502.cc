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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc(def)ghi";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    int never_nl_val = 0;
    int current_ver = 0;
    const char *pat_from_re = NULL;
    int first_char = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    never_nl_val = cre2_opt_never_nl(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    current_ver = cre2_version_interface_current();
    first_char = (int)pat_from_re[0];

    // step 5: Validate
    computed_check = current_ver + never_nl_val + first_char;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}