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
//<ID> 1993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "abc(def)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    int perl_before = 0;
    int perl_after = 0;
    int64_t requested_max_mem = (int64_t)(16 * 1024 * 1024);
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    perl_before = cre2_opt_perl_classes(opt);
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_max_mem(opt, requested_max_mem);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    perl_after = cre2_opt_perl_classes(opt);

    // step 4: Validate
    checksum = (long)pattern_len + (long)(pattern_from_re != NULL) + (long)perl_before + (long)perl_after + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}