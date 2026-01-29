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
//<ID> 416
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|foobar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|hello";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    const char *extracted_pattern = NULL;
    int longest_flag = 0;
    int add_res = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_longest_match(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    add_res = cre2_set_add_simple(set, set_pattern);

    // step 4: Operate
    longest_flag = cre2_opt_longest_match(opt);
    extracted_pattern = cre2_pattern(re);

    // step 5: Validate
    computed_check = longest_flag + add_res + pattern_len + (int)extracted_pattern[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}