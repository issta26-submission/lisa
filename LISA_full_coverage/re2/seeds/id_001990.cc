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
//<ID> 1990
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * observed_pattern = NULL;
    int perl_classes_flag = 0;
    long long max_mem_ll = 1024LL * 1024LL; // 1 MiB
    int checksum = 0;

    // step 2: Setup - create options and configure memory and literal mode
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)max_mem_ll);
    cre2_opt_set_literal(opt, 1);

    // step 3: Configure - query an option to ensure accessor usage
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 4: Operate - compile a regexp using the configured options
    re = cre2_new(pattern, pattern_len, opt);

    // step 5: Inspect - retrieve the pattern string from compiled regexp
    observed_pattern = cre2_pattern(re);

    // step 6: Validate and Cleanup
    checksum = pattern_len + (int)perl_classes_flag + (int)(opt != NULL) + (int)(re != NULL) + (int)(observed_pattern != NULL) + (int)(max_mem_ll & 0xFFFF);
    (void)checksum;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}