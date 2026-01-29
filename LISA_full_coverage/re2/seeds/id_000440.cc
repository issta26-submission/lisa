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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "foo|bar|baz";
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    int one_line_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int added = 0;
    int compiled = 0;
    int num_caps = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);
    added = cre2_set_add_simple(set, set_pattern);
    compiled = cre2_set_compile(set);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    num_caps = cre2_num_capturing_groups(re);
    cre2_named_groups_iter_delete(iter);

    // step 5: Validate
    one_line_flag = cre2_opt_one_line(opt);
    enc = cre2_opt_encoding(opt);
    computed_check = added + compiled + num_caps + one_line_flag + (int)enc;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}