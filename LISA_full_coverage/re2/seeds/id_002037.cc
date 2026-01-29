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
//<ID> 2037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(hello|world)[0-9]{1,3}$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const int64_t configured_max_mem = (int64_t)4 * 1024 * 1024;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int error_code = 0;
    long checksum = 0;

    // step 2: Initialize
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_max_mem(opt, configured_max_mem);

    // step 4: Operate
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Validate
    error_code = cre2_error_code(re);
    checksum = (long)pattern_len + (long)configured_max_mem + (long)error_code + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}