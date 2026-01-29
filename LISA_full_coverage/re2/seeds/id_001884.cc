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
//<ID> 1884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(alpha)(beta)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * pat_out = NULL;
    int num_groups = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Core operations
    pat_out = cre2_pattern(re);
    num_groups = cre2_num_capturing_groups(re);

    // step 4: Validate
    checksum = (long)pattern_len + (long)enc + (long)(pat_out != NULL) + (long)num_groups + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}