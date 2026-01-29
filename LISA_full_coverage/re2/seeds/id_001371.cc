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
//<ID> 1371
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
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int64_t max_mem_val = 0;
    int literal_flag = 0;
    int ver_interface = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_max_mem(opt, (int64_t) (16 * 1024 * 1024)); // set 16MB max memory
    cre2_opt_set_literal(opt, 1); // force literal mode for the pattern
    max_mem_val = cre2_opt_max_mem(opt);
    literal_flag = cre2_opt_literal(opt);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 5: Validate
    ver_interface = cre2_version_interface_current();
    computed_check = (long)pattern_len + (long)max_mem_val + (long)literal_flag
                     + (long)(re != NULL) + (long)(iter != NULL) + (long)ver_interface;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}