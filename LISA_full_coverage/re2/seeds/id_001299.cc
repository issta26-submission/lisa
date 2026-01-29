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
//<ID> 1299
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc.*def";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    char scratch[32];
    long computed_check = 0;
    int perl_classes_flag = 0;
    int64_t max_mem_val = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)4 * 1024 * 1024);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate / compute lightweight checksum
    perl_classes_flag = cre2_opt_perl_classes(opt);
    max_mem_val = cre2_opt_max_mem(opt);
    computed_check = (long)pattern_len + (long)(re != NULL) + (long)perl_classes_flag + (long)max_mem_val + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}