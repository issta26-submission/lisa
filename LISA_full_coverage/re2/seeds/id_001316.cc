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
//<ID> 1316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * re_pat = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    int revision = 0;
    char scratch[32];
    long computed_check = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);

    // step 4: Operate / Query
    (void)cre2_named_groups_iter_next(iter, &found_name, &found_index);
    revision = cre2_version_interface_revision();

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)(re_pat != NULL)
                     + (long)revision + (long)(found_name != NULL) + (long)found_index;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}