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
//<ID> 1992
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(bc)d+";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    int perl_classes_flag = 0;
    int literal_flag = 1;
    long long max_mem_ll = 4LL * 1024LL * 1024LL; // 4MB
    const char * returned_pattern = NULL;
    long checksum = 0;

    // step 2: Setup - create and configure options
    opt = cre2_opt_new();
    cre2_opt_set_max_mem(opt, (int64_t)max_mem_ll);
    cre2_opt_set_literal(opt, literal_flag);
    perl_classes_flag = cre2_opt_perl_classes(opt);

    // step 3: Operate - compile the pattern with configured options
    re = cre2_new(pattern, pattern_len, opt);
    returned_pattern = cre2_pattern(re);

    // step 4: Validate - compute a checksum that depends on earlier results
    checksum = (long)pattern_len + (long)perl_classes_flag + (long)literal_flag + (long)(max_mem_ll % 1000000LL) + (long)(re != NULL) + (long)(returned_pattern != NULL);
    (void)checksum;

    // step 5: Cleanup - free resources
    cre2_delete(re);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}