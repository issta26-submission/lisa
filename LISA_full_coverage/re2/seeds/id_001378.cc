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
//<ID> 1378
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *pattern_from_re = NULL;
    int literal_flag = 0;
    int ver_interface = 0;
    int64_t max_mem = 0;
    long computed_check = 0L;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    literal_flag = cre2_opt_literal(opt);
    max_mem = cre2_opt_max_mem(opt);
    ver_interface = cre2_version_interface_current();

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)(iter != NULL)
                     + (long)literal_flag + (long)max_mem + (long)ver_interface
                     + (long)(pattern_from_re != NULL);
    (void)computed_check;
    (void)pattern_from_re;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}