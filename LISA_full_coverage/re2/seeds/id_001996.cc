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
//<ID> 1996
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pattern_from_re = NULL;
    int perl_classes_flag = 0;
    char scratch[32];
    int64_t max_memory = (int64_t)(8LL * 1024LL * 1024LL);
    long checksum = 0;

    // step 2: Setup
    memset(scratch, 0, sizeof(scratch));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_max_mem(opt, max_memory);

    // step 3: Operate - query option and compile regexp
    perl_classes_flag = cre2_opt_perl_classes(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate - inspect compiled regexp
    pattern_from_re = cre2_pattern(re);

    // step 5: Validate (aggregate values to exercise variables)
    checksum = (long)pattern_len + (long)perl_classes_flag + (long)(pattern_from_re != NULL) + (long)(opt != NULL) + (long)(re != NULL) + (long)(max_memory / 1024);

    // step 6: Cleanup
    (void)checksum;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}