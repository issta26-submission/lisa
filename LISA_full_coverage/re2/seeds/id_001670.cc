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
//<ID> 1670
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
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    const char *compiled_pat = NULL;
    int num_groups = 0;
    char pat_copy[64];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_never_nl(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    compiled_pat = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    num_groups = cre2_num_capturing_groups(re);
    memset(pat_copy, 0, sizeof(pat_copy));
    if (compiled_pat != NULL) {
        /* copy up to buffer size - 1 to ensure null termination without including headers */
        int copy_len = pattern_len;
        if (copy_len > (int)sizeof(pat_copy) - 1) copy_len = (int)sizeof(pat_copy) - 1;
        memcpy(pat_copy, compiled_pat, (size_t)copy_len);
        pat_copy[copy_len] = '\0';
    }

    // step 5: Validate
    checksum = (long)pattern_len + (long)num_groups + (long)(compiled_pat != NULL) + (long)(iter != NULL) + (long)pat_copy[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}