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
//<ID> 1999
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)[0-9]+$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    int perl_classes_flag = 0;
    long long max_mem_value = 1024LL * 1024LL;
    long checksum = 0;

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_max_mem(opt, max_mem_value);
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 3: Operate - compile regexp with configured options
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);

    // step 4: Validate - compute a simple checksum based on results
    checksum = (long)pattern_len + (long)max_mem_value + (long)perl_classes_flag + (long)(opt != NULL) + (long)(re != NULL) + (long)(pattern_from_re != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}